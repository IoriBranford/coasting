/// <reference types="@mapeditor/tiled-api" />

var customMapFormat = {
    name: "Neon Coaster course code",
    extension: "c",

    write: function(/** @type TileMap */map, /**@type string */ fileName) {
        let mapName = fileName.replace(/^.*[\\\/]/, '').replace(/\.c$/, '');
        var hlines = [
            "#ifndef __"+mapName+"_H",
            "#define __"+mapName+"_H",
            "#include \"track.h\"",
            "#define NUM_COURSES "+map.layerCount,
        ];
        var clines = [
            "#include \""+mapName+".h\"",
        ];

        for (var i = 0; i < map.layerCount; ++i) {
            /**@type ObjectGroup */ var layer = map.layerAt(i);
            if (!layer.isObjectLayer || !layer.name == "courses") {
                continue;
            }

            layer.objects.forEach(object => {
                if (object.shape != MapObject.Polyline){
                    return;
                }

                let name = object.name.toUpperCase().replace(/[^_A-Za-z0-9]/gi, '');
                if (name.length < 1) {
                    name = i.toString();
                }

                clines.push("Track COURSE_"+name+"[] = {");
                let x = Math.floor(object.x);
                let y = Math.floor(object.y);
                let size = object.polygon.length;
                object.polygon.forEach(point => {
                    let dx = Math.floor(point.x) - x;
                    let dy = Math.floor(point.y) - y;
                    if (dx != 0 || dy != 0) {
                        clines.push("{ .dx = "+dx+", .dy = "+dy+" },")
                        x += dx;
                        y += dy;
                    } else {
                        --size;
                    }
                });
                clines.push("};");

                hlines.push("#define COURSE_"+name+"_SIZE "+size);
                hlines.push("extern Track COURSE_"+name+"[];");
            })
            break;
        }
        hlines.push("#endif");
        var cfile = new TextFile(fileName, TextFile.WriteOnly);
        cfile.writeLine(clines.join("\n"));
        cfile.commit();
        var hfile = new TextFile(fileName.replace(/\.c$/, ".h"), TextFile.WriteOnly);
        hfile.writeLine(hlines.join("\n"));
        hfile.commit();
    },
}

tiled.registerMapFormat("custom", customMapFormat)
