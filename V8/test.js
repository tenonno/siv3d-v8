


siv3d.main = async (...args) => {


    while (await siv3d.update()) {

        siv3d.setBackgroundHue(siv3d.frameCount);
       
        siv3d.println(args[0], '��������������', siv3d.frameCount);

    }


};
