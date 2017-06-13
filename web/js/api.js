// Agenda Web Function

// auto call function
$(function () {
  $('#agenda-nav-bar').load('nav-bar.html');
});


// sign up function
function signup() {
  if ($('#in-username').val() == '' || $('#in-email').val() == '' || $('#in-password').val() == '') {
    return false;
  }
  $.post('/api/agenda/signup', {
    username: $('#in-username').val(),
    email: $('#in-email').val(),
    password: $('#in-password').val()
  }, (data) => {
    if (data.status == 'success') {
      $.cookie('cookie_name', $('#in-username').val());
      $.cookie('cookie_id', data.id);
      window.location.reload();
    }
  });
  return true;
}


// sign in function
function sigin() {
  if ($('#in-username').val() == '' || $('#in-password').val() == '') {
    return false;
  }
  $.post('/api/agenda/login', {
    username: $('#in-username').val(),
    password: $('#in-password').val()
  }, (data) => {

  });
  return true;
}

 
// log out function
function logout() {
  $.cookie('cookie_name', null);
  $.cookie('cookie_id', 0);
  $('#nav-username').html('No sign in'+'<span class="caret"></span>');
  window.location.reload();
}

