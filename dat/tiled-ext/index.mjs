/// <reference types="@mapeditor/tiled-api" />

let toFixed = i => Math.floor(i*4096)
let toFixedAngle = a => Math.floor(a*2048/Math.PI)

let findCourses = (/** @type TileMap */map) => {
    let courses = [];
    for (var i = 0; i < map.layerCount; ++i) {
        /**@type ObjectGroup */ var layer = map.layerAt(i);
        if (!layer.isObjectLayer)
            continue;

        let x = 0;
        let y = 0;
        let tracks = [];
        let fCourseLength = 0;
        let message = "";
        layer.objects.forEach(object => {
            if (object.shape == MapObject.Polyline) {
                let ox = Math.floor(object.x);
                let oy = Math.floor(object.y);
                object.polygon.forEach(point => {
                    let dx = Math.floor(point.x) + ox - x;
                    let dy = Math.floor(point.y) + oy - y;
                    if (dx != 0 || dy != 0) {
                        let fTrackLength = toFixed(Math.hypot(dx, dy));
                        tracks.push({
                            dx: dx, dy: dy,
                            x0: x, y0: y,
                            f_start: fCourseLength,
                            f_len: fTrackLength,
                        })
                        x += dx;
                        y += dy;
                        fCourseLength += fTrackLength;
                    }
                })
            } else if (object.shape == MapObject.Text) {
                message = object.text;
            }
        })

        if (fCourseLength > 0) {
            let course = {
                f_len: fCourseLength,
                tracks: tracks,
                message: message,
            };
            let courseName = layer.name.replace(/[^_A-Za-z0-9]/gi, '');
            if (courseName.length > 0) {
                course.name = courseName;
            }

            courses.push(course);
        }
    }
    return courses;
}

let buildCourseCode = (courses, fileName) => {
    let mapName = fileName.replace(/^.*[\\\/]/, '').replace(/\.c$/, '');
    let MAPNAME = mapName.toUpperCase()
    var headerlines = [
        "#ifndef __"+MAPNAME+"_H",
        "#define __"+MAPNAME+"_H",

        "#include \"course.h\"",
        "#define NUM_COURSES "+courses.length,
        "extern Course "+MAPNAME+"[];",
    ];

    var sourcelines = [
        "#include \""+mapName+".h\"",
    ];

    courses.forEach((course, i) => {
        if (!course.name) {
            course.name = i.toString();
        }
        headerlines.push(`#define COURSE_${course.name} ${i}`);
        sourcelines.push(`static Track TRACKS_${course.name}[] = {`);
        course.tracks.forEach(track => {
            sourcelines.push("{")
            sourcelines.push(`.dx = ${track.dx}, .dy = ${track.dy},`)
            sourcelines.push(`.x0 = ${track.x0}, .y0 = ${track.y0},`)
            sourcelines.push(`.f_start = ${track.f_start}, .f_len = ${track.f_len},`)
            
            let f_angle = toFixedAngle(Math.atan2(track.dy, track.dx));
            let f_dirx = toFixed(toFixed(track.dx)/track.f_len);
            let f_diry = toFixed(toFixed(track.dy)/track.f_len);
            sourcelines.push(`.f_angle = ${f_angle}, .f_dirx = ${f_dirx}, .f_diry = ${f_diry},`)
            sourcelines.push("},")
        })
        sourcelines.push("};");
    })

    sourcelines.push(`Course ${MAPNAME}[] = {`);
    courses.forEach((course) => {
        sourcelines.push("{")
        sourcelines.push(`.name = "${course.name}",`)
        sourcelines.push(`.message = "${course.message}",`)
        sourcelines.push(`.f_len = ${course.f_len},`)
        sourcelines.push(`.num_tracks = ${course.tracks.length},`)
        sourcelines.push(`.tracks = TRACKS_${course.name}`)
        sourcelines.push("},")
    })
    sourcelines.push("};");

    headerlines.push("#endif");
    return [sourcelines.join("\n"), headerlines.join("\n")]
}

var customMapFormat = {
    name: "Neon Coaster course code",
    extension: "c",

    write: function(/** @type TileMap */map, /**@type string */ fileName) {
        let courses = findCourses(map);
        let [source, header] = buildCourseCode(courses, fileName);

        var cfile = new TextFile(fileName, TextFile.WriteOnly);
        cfile.writeLine(source);
        cfile.commit();
        var hfile = new TextFile(fileName.replace(/\.c$/, ".h"), TextFile.WriteOnly);
        hfile.writeLine(header);
        hfile.commit();
    },
}

tiled.registerMapFormat("custom", customMapFormat)
