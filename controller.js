

function keyPress() {
    console.log('meow');
    let evt = new KeyboardEvent("keydown", {
        shiftKey: false,
        key: "1",
        keyCode: 49
    });
    document.dispatchEvent(evt);
}