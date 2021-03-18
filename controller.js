function getSearch() {
    var radios = document.getElementsByName('search');

    for (var i = 0, length = radios.length; i < length; i++) {
        if (radios[i].checked) {
            //console.log(radios[i].value);
            return radios[i].value;
        }
    }
}

function getSpeed() {
    //console.log(document.getElementById("speed").value);
    return document.getElementById("speed").value;
}