

siv3d.main = async (...args) => {

    siv3d.println(`length: ${args.length}`);

    args.forEach(value => siv3d.println(value));


    let count = 0;
    let value = null;
    while (value = await system.update()) {

        siv3d.println(value);

        if (count++ > 100) break;

    }




};

siv3d.println(system.update);




