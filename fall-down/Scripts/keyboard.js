var Key = {
  _pressed: {},
  
  isDown: function(keyCode) {
    return !!this._pressed[keyCode];
  },
  
  press: function(which) {
    this._pressed[which] = true;
  },
  
  release: function(which) {
    this._pressed[which] = undefined;
  }
};

(function () {
    var i, c;
    var keys = 'WASD ZXCVFIJKL';
    for (var i in keys)
    {
        c = keys[i];
        Key[c == ' ' ? 'SPACE' : c] = c.charCodeAt(0);
    }
})();

jQuery(document).on(
    'keydown',
    function (e) {
       Key.press(e.which);
    });
jQuery(document).on(
    'keyup',
    function (e) {
        Key.release(e.which);
    });
