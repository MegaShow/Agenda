// Agenda Web Function

// auto call function
$(() => {
  if ($.cookie('cookie_name') != null && $.cookie('cookie_name') != 'null') {
    $('#nav-username').html($.cookie('cookie_name')+'<span class="caret"></span>');
  } else {
    $('#nav-username').html('No sign in'+'<span class="caret"></span>');
  }
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
      $('#nav-username').html($('#in-username').val()+'<span class="caret"></span>');
    }
  });
  return true;
}


// sign in function
function sigin() {

}

 
// log out function
function logout() {
  $.cookie('cookie_name', '');
  $.cookie('cookie_id', '');
  $('#nav-username').html('No sign in'+'<span class="caret"></span>');
}

