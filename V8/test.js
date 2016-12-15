

siv3d.main = async (...args) => {

    siv3d.println(t.test);

    // siv3d.func();

    while (await system.update()) {

       
        siv3d.println(siv3d.frameCount);

    }


};

