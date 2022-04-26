import QtQuick 2.15
import app 1.0;
//import "../optiMAL/main.mjs" as Main
import "./main.mjs" as Main

Item {

    id: root

    /*Logger{
        id: logger
    }*/

    property string a: "A"
    property string b: "B"

    ApplicationData {
        id: appData0
    }

    ApplicationFactory {
        id: glob
    }

    function main(args) {
        //logger.log(Logger.Debug, "Entering function aFunctionThatYouWantToDebug(" + iArgumentOne + ", " + iArgumentTwo + ")");
        //console.dir("abc");
        console.log(args);
        console.log(glob);
        glob.log(["abc\nxyz", function() {}])
        glob.log(typeof function(){});
        console.log(args)
        console.log(args.length)
        console.log(args[0]);
        console.log("main "+a+b)
        console.log(appData0.getTextFromCpp());
        let appData = glob.newApplicationData();
        console.log(appData);
        console.log(appData.dump());
        console.log(appData.getTextFromCpp());
        glob.log(appData.retVariant());
        var obj = appData.retVariant2();
        glob.log(obj);
        var list = obj["b"];
        glob.log(list);
        for(var x of list)
        {
            if(x != null) console.log(x);
        }

        Main.main(glob);
        return 777;
    }

    Timer {
        running: true
        interval: 0
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            try {
                let exitCode = root.main(Qt.application.arguments);
                if(!Number.isInteger(exitCode)) exitCode = 0;
                Qt.exit(exitCode);
            } catch(e) {
                Qt.exit(-1);
                throw e;
            }
        }
    }

}
