// Agenda Web Function


$(() => {
  if (Cookies.get('cookie_name') != null) {
    $('#nav-username').html(Cookies.get('cookie_name'));
  } else {
    $('#nav-username').html('No sign in');
  }
});


function signup() {
  if ($('#username').val() == '' || $('#email').val() == '' || $('#password').val() == '') {
    return false;
  }
  $.post('/api/agenda/signup', {
    username: $('#username').val(),
    email: $('#email').val(),
    password: $('#password').val()
  }, (data) => {
    if (data.status == 'success') {
      Cookies.set('cookie_name', username);
      Cookies.set('cookie_id', data.id);
      $('#nav-username').html(username);
    }
  });
  return true;
}

