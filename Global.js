.pragma library
var active = false;
var email = "";
var password = "";
var path = "";


function setJDataUser(b) {
    var data = JSON.parse(b);
    email = data.email;
    password = data.password;
    path = data.path;
}

function scrollViewToEnd() {
    var newIndex = listView.count;
    console.log(newIndex);
}
