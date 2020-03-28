
$.get = function(uri) {
    return $.invoke(uri);
}

$.post = function(uri, body) {
    return $.invoke(uri, body);
}

$.qs = function(uri, data) {
    if(data === undefined) {
        return $.addConnect(uri);
    }else{
        return $.addConnect(uri, data);
    }
}
