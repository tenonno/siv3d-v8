

siv3d.main = async (...args) => {


    while (await system.update()) {

        siv3d.println(siv3d.frameCount);

    }


};

