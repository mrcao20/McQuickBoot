$.__proto__.get = function(uri, callback) {
    return $.invoke(uri, callback);
}

$.__proto__.post = function(uri, body, callback) {
    return $.invoke(uri, body, callback);
}

String.prototype.format = function(args) {
    if(arguments.length <= 0) {
        return this;
    }

    var result = this;
    if(arguments.length == 1 && typeof(args) == 'object') {
        for(var key in args) {
            if(args[key] !== undefined) {
                var reg = new RegExp('({' + key + '})', 'g');
                result = result.replace(reg, args[key]);
            }
        }
    }else{
        for(var i = 0; i < arguments.length; ++i) {
            if(arguments[i] !== undefined) {
                reg = new RegExp('({)' + i + '(})', 'g');
                result = result.replace(reg, arguments[i]);
            }
        }
    }
    return result;
}
