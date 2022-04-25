function func() {
    console.log("func(main.js) called!");
    let appData = factory.newApplicationData();
    console.log(appData.getTextFromCpp());
}
