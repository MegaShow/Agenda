// Agenda Web Function

// auto call function
$(function () {
  $('#agenda-nav-bar').load('nav-bar.html');
});


// sign up function
function signup() {
  if ($('#agenda-sign-up-username').val() == '' || $('#agenda-sign-up-password').val() == '' 
    || $('#agenda-sign-up-confirm-password').val() == '' || $('#agenda-sign-up-email').val() == ''
    || $('#agenda-sign-up-phone').val() == '') {
    return false;
  } else if ($('#agenda-sign-up-password').val() != $('#agenda-sign-up-confirm-password').val()) {
    message('Comfirm failed!');
    return false;
  }
  $.post('/api/agenda/signup', {
    username: $('#agenda-sign-up-username').val(),
    password: $('#agenda-sign-up-password').val(),
    email: $('#agenda-sign-up-email').val(),
    phone: $('#agenda-sign-up-phone').val()
  }, (data) => {
    if (data.status == 'success') {
      $.cookie('cookie_name', $('#agenda-sign-up-username').val());
      $.cookie('cookie_id', data.id);
      window.location.href="User.html";
    } else {
      message(data.err);
      return false;
    }
  });
  return true;
}


function modify() {
  if ($('#modify-email').val() == '' || $('#modify-phone').val() == '') {
    return false;
  }
  $.post('/api/agenda/modify', {
    id: $.cookie('cookie_id'),
    email: $('#modify-email').val(),
    phone: $('#modify-phone').val()
  }, (data) => {
    window.location.reload();
  });
  return true;
}

function setting() {
  if ($('#setting-old-password').val() == '' || $('#setting-new-password').val() == '' || $('#setting-again-password').val() == '') {
    message('Incomplete input!');
    return false;
  } else if ($('#setting-new-password').val() != $('#setting-again-password').val()) {
    message('Comfirm failed!');
    return false;
  }
  $.post('/api/agenda/setting', {
    id: $.cookie('cookie_id'),
    old_password: $('#setting-old-password').val(),
    new_password: $('#setting-new-password').val()
  }, (data) => {
    if (data.status == 'success') {
      message('Sucessfully setting!');
    } else {
      message(data.err);
    }
  });
  return true;
}


// sign in function
function signin() {
  if ($('#in-username').val() == '' || $('#in-password').val() == '') {
    return false;
  }
  $.post('/api/agenda/login', {
    username: $('#in-username').val(),
    password: $('#in-password').val()
  }, (data) => {
    if (data.status == 'success') {
      $.cookie('cookie_name', $('#in-username').val());
      $.cookie('cookie_id', data.id);
      window.location.href = "User.html";
    } else {
      message(data.err);
    }
  });
  return true;
}

 
// log out function
function logout() {
  $.cookie('cookie_name', null);
  $.cookie('cookie_id', 0);
  window.location.href = "SignIn.html";
}

//delete
function signoff() {
  if ($('#delete-name').val() == '' || $('#delete-password').val() == '') {
    return false;
  }
  $.post('/api/agenda/delete', {
    id: $.cookie('cookie_id'),
    name: $('#delete-name').val(),
    password: $('#delete-password').val()
  }, (data) => {
    if (data.status == 'success') {
      $.cookie('cookie_name', null);
      $.cookie('cookie_id', 0);
      window.location.href = "SignIn.html";
    } else {
      message(data.err);
    }
  });
  return true;
}
