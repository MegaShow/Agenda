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
    if (data.status == 'successful') {
      $.cookie('cookie_name', $('#agenda-sign-up-username').val());
      $.cookie('cookie_id', data.id);
      window.location.href = "User.html";
      return true;
    } else {
      message(data.err);
      return false;
    }
  });
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
    if (data.status == 'successful') {
      $.cookie('cookie_name', $('#in-username').val());
      $.cookie('cookie_id', data.id);
      window.location.href = "User.html";
      return true;
    } else {
      message(data.err);
      return false;
    }
  });
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
    return false;
  } else if ($('#setting-new-password').val() != $('#setting-again-password').val()) {
    message('Comfirm password error!');
    return false;
  }
  $.post('/api/agenda/setting', {
    id: $.cookie('cookie_id'),
    old_password: $('#setting-old-password').val(),
    new_password: $('#setting-new-password').val()
  }, (data) => {
    if (data.status == 'successful') {
      window.location.reload();
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
  $.post('/api/agenda/signoff', {
    id: $.cookie('cookie_id'),
    name: $('#delete-name').val(),
    password: $('#delete-password').val()
  }, (data) => {
    if (data.status == 'successful') {
      $.cookie('cookie_name', null);
      $.cookie('cookie_id', 0);
      window.location.href = "SignIn.html";
    } else {
      message(data.err);
    }
  });
  return true;
}

// create meeting
function createMeeting() {
  if ($('#agenda-create-title').val() == '' ||
    $('#agenda-create-start-time-val').val() == '' ||
    $('#agenda-create-end-time-val').val() == '') {
    return false;
  }
  $.post('/api/agenda/user/', {}, (data) => {
    if (data.status == 'successful') {
      var part = '';
      for (const i of data.user) {
        if (i.name != $.cookie('cookie_name')) {
          if ($('input[value="'+i.name+'"]').is(':checked') == true) {
            part += i.name + ',';
            var flag = true;
            $.post('/api/agenda/hastime', {
              username: i.name,
              start: $('#agenda-create-start-time-val').val(),
              end: $('#agenda-create-end-time-val').val()
            }, (data) => {
              if (data.status == 'failed') {
                flag = false;
              }
            });
            if (flag == false) {
              message('<span class="glyphicon glyphicon-user" aria-hidden="true"></span>' + i.name + ' has no time!');
              return false;
            }
          }
        }
      }
      if (part == '') {
        return false;
      }
      $.post('/api/agenda/create/', {
        title: $('#agenda-create-title').val(),
        sponsor: $.cookie('cookie_name'),
        participator: part,
        start: $('#agenda-create-start-time-val').val(),
        end: $('#agenda-create-end-time-val').val(),        
      }, (data) => {
        if (data.status == 'successful') {
          window.location.reload();
        } else {
          message(data.err);
        }
      });
    }
  });
}

