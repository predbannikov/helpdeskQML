.pragma library
var active = false;
var email = "";
var password = "";
var path = "";
var indexStart = 0;
var indexEnd = 0;

//  For editor ticket
//var curIdToEdit = 0;

var itemListView = {
    id: 0,
    name: "",
    status_id: ""
}
function setItem(a, b, c) {
    itemListView.id = a;
    itemListView.name = b;
    itemListView.status_id = c;
}

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

function getAbsolutePosition(node) {
    var returnPos = {};
    returnPos.x = 0;
    returnPos.y = 0;
    if(node !== undefined && node !== null) {
        var parentValue = getAbsolutePosition(node.parent);
        returnPos.x = parentValue.x + node.x;
        returnPos.y = parentValue.y + node.y;
    }
    return returnPos;
}
