// Agenda Web API
const express = require('express');
const app = express();
const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: false }));
const sqlModule = require('./mysql.js'); //数据库模块


// 注册模块
app.post('/agenda/signup', (req, res, next) => {
  var id;
  var username = sqlModule.dealEscape(req.body.username);
  var password = sqlModule.dealEscape(req.body.password);
  var email = sqlModule.dealEscape(req.body.email);
  var phone = sqlModule.dealEscape(req.body.phone);
  console.log('Event: User Sign Up');
  console.log('Username: ' + username);
  console.log('Password: ' + password);
  console.log('Email: ' + email);
  console.log('Phone: ' + phone);
  sqlModule.query("SELECT * FROM `user` WHERE binary `name` = '" + username + "';", (vals, isNull) => {
    if (isNull) {
      sqlModule.query("SELECT * FROM `globe` WHERE `name` = 'user';", (vals, isNull) => {
        if (isNull) {
          console.log('Select globe user error!!!');
          res.send({ status: 'failed' });
        } else {
          id = vals[0].var;
          id++;
          console.log('id: ' + id);
          sqlModule.query("UPDATE `globe` SET `var` = '" + id + "' WHERE `globe`.`keyvar` = 0;");
          sqlModule.query("INSERT INTO `user` (`id`, `name`, `email`, `password`, `phone`) \
                          VALUES ('"+ id + "', '" + username + "', '" + email + "', '" + password + "', '" + phone + "');");
          console.log('Successful.\n');
          res.send({ status: 'successful', id: id });
        }
      });
    } else {
      console.log('Failed. The username has been used.\n');
      res.send({ status: 'failed', err: 'The username has been used!' });
    }
  });
});

// 登录模块
app.post('/agenda/login', (req, res, next) => {
  var id;
  var username = sqlModule.dealEscape(req.body.username);
  var password = sqlModule.dealEscape(req.body.password);
  console.log('Event: User Sign In');
  console.log('User: ' + username);
  console.log('Password: ' + password);
  sqlModule.query("SELECT * FROM `user` WHERE binary `name` = '" + username + "';", (vals, isNull) => {
    if (isNull) {
      console.log('User doesn\'t exist.\n');
      res.send({ status: 'failed', err: 'User doesn\'t exist!' });
    } else if (vals[0].password == password) {
      id = vals[0].id;
      console.log('Successful.\n');
      res.send({ status: 'successful', id: id });
    } else {
      console.log('Failed. Password error.\n');
      res.send({ status: 'failed', err: 'Password error!' });
    }
  });
});

//userInfo
app.post('/agenda/userInfo', (req, res, next) => {
  var id = sqlModule.dealEscape(req.body.id);
  console.log('Event: User Info');
  console.log('ID: ' + id + '\n');
  sqlModule.query("SELECT * FROM `user` WHERE `id` = " + id + ";", (vals, isNull) => {
    if (isNull) {
      console.log('User Info failed.\n');
      res.send({ status: 'failed' });
    } else {
      _email = vals[0].email;
      _phone = vals[0].phone;
      console.log('User Info successful.\n');
      res.send({ status: 'successful', email: _email, phone: _phone });
    }
  });
});

//modify
app.post('/agenda/modify', (req, res, next) => {
  var id = sqlModule.dealEscape(req.body.id);
  var email = sqlModule.dealEscape(req.body.email);
  var phone = sqlModule.dealEscape(req.body.phone);
  console.log('Event: User Modify');
  console.log('ID: ' + id);
  console.log('Email: ' + email);
  console.log('Phone: ' + phone);
  sqlModule.query("UPDATE `user` SET `email` = '" + email + "', `phone` = '" + phone + "' WHERE `user`.`id` = " + id + ";");
  res.send({});
});

//setting
app.post('/agenda/setting', (req, res, next) => {
  var id = sqlModule.dealEscape(req.body.id);
  var old_password = sqlModule.dealEscape(req.body.old_password);
  var new_password = sqlModule.dealEscape(req.body.new_password);
  console.log('Event: User Setting');
  console.log('ID: ' + id);
  console.log('Old Password: ' + old_password);
  console.log('New Password: ' + new_password);
  sqlModule.query("SELECT * FROM `user` WHERE `id` = " + id + ";", (vals, isNull) => {
    if (isNull) {
      console.log('Failed. User doesn\'t exist.\n');
      res.send({ status: 'failed', err: 'User doesn\'t exist!' });
    } else if (vals[0].password != old_password) {
      console.log('Failed. Password error.\n');
      res.send({ status: 'failed', err: 'Password error!' });
    } else {
      sqlModule.query("UPDATE `user` SET `password` = '" + new_password + "' WHERE `user`.`id` = " + id + ";");
      console.log('Successful.\n');
      res.send({ status: 'successful' });
    }
  });
});

//signoff
app.post('/agenda/signoff', (req, res, next) => {
  var id = sqlModule.dealEscape(req.body.id);
  var name = sqlModule.dealEscape(req.body.name);
  var password = sqlModule.dealEscape(req.body.password);
  console.log('Event: User Sign Off');
  console.log('ID: ' + id);
  console.log('Name: ' + name);
  console.log('Password: ' + password);
  sqlModule.query("SELECT * FROM `user` WHERE `id` = " + id + ";", (vals, isNull) => {
    if (isNull) {
      console.log('Failed. User doesn\'t exist.\n');
      res.send({ status: 'failed', err: 'User doesn\'t exist!' });
    } else if (vals[0].name != name) {
      console.log('Failed. Username error.\n');
      res.send({ status: 'failed', err: 'Username error!' });
    } else if (vals[0].password != password) {
      console.log('Failed. Password error.\n');
      res.send({ status: 'failed', err: 'Password error!' });
    } else {
      sqlModule.query("DELETE FROM `user` WHERE `user`.`id` = " + id + ";");
      console.log('Successful.\n');
      res.send({ status: 'successful' });
    }
  });
});

app.post('/agenda/sponsor_meeting', (req, res, next) => {
  var name = sqlModule.dealEscape(req.body.name);
  console.log('Event: Get Sponsor Meeting');
  console.log('Name: ' + name);
  sqlModule.query("SELECT * FROM `meeting` WHERE binary `sponsor` LIKE '" + name + "' ORDER BY `sponsor` ASC", (vals, isNull) => {
    if (isNull) {
      console.log('Sponsor failed. No sponsor meeting.\n');
      res.send({ status: 'failed' });
    } else {
      console.log('Sponsor successful.\n');
      res.send({ status: 'successful', meetings: vals });
    }
  })
});

app.post('/agenda/part_meeting', (req, res, next) => {
  var name = sqlModule.dealEscape(req.body.name);
  console.log('Event: Get Participator Meeting');
  console.log('Name: ' + name);
  sqlModule.query("SELECT * FROM `meeting` WHERE binary `part` LIKE '%" + name + "%' ORDER BY `sponsor` ASC", (vals, isNull) => {
    if (isNull) {
      console.log('Paticipator failed. No paticipator meeting.\n');
      res.send({ status: 'failed' });
    } else {
      console.log('Sponsor successful.\n');
      res.send({ status: 'successful', meetings: vals });
    }
  })
});

// 获取会议信息
app.post('/agenda/meeting', (req, res, next) => {
  console.log('Event: Get Meeting List');
  sqlModule.query("SELECT * FROM `meeting`;", (vals, isNull) => {
    if (isNull) {
      console.log('No meeting\n');
      res.send({ status: 'failed' });
    } else {
      console.log('Success\n');
      res.send({ status: 'successful', meeting: vals });
    }
  })
});


// 获取用户信息
app.post('/agenda/user', (req, res, next) => {
  console.log('Event: Get User List');
  sqlModule.query("SELECT * FROM `user`;", (vals, isNull) => {
    if (isNull) {
      console.log('No user\n');
      res.send({ status: 'failed' });
    } else {
      console.log('Success\n');
      res.send({ status: 'successful', user: vals });
    }
  })
});


app.post('/agenda/create', (req, res, next) => {
  var sponsor = sqlModule.dealEscape(req.body.sponsor);
  var title = sqlModule.dealEscape(req.body.title);
  var part = sqlModule.dealEscape(req.body.participator);
  var start = sqlModule.dealEscape(req.body.start);
  var end = sqlModule.dealEscape(req.body.end);
  console.log('Event: Create Meeting');
  console.log('Sponsor: ' + sponsor);
  console.log('Title: ' + title);
  console.log('Participator: ' + part);
  console.log('Start Time: ' + start);
  console.log('End Time: ' + end);
      //？
  var val;
  sqlModule.query("SELECT * FROM `meeting` WHERE `start` < '" + end + "' AND `end` > '" + start + "';", (vals, isNull) => {
    var flag = true;
    var test_part = '';
    if (isNull) {
      flag = true;
    } else {
      for (var i in vals) {
        if (i == 0) {
          test_part += vals[i].part;
        } else {
          test_part += ',';
          test_part += vals[i].part;
        }
        test_part += ',';
        test_part += vals[i].sponsor;
      }
      var arr = (part + ',' + sponsor).split(',');
      var test_arr = test_part.split(',');
      for (var i of arr) {
        for (var j of test_arr) {
          if (i == j) {
            flag = false;
            break;
          }
        }
        if (flag == false) {
          break;
        }
      }
    }
    if (flag === false) {
      console.log('Failed. Some users have no time.');
      res.send({ status: 'failed', err: 'Some users have no time!' });
    } else {
      sqlModule.query("SELECT * FROM `meeting` WHERE binary `title` = '" + title + "';", (vals, isNull) => {
        if (isNull) {
          sqlModule.query("SELECT * FROM `globe` WHERE `name` = 'meeting';", (vals, isNull) => {
            if (isNull) {
              console.log('Select globe meeting error!!!');
              res.send({ status: 'failed' });
            } else {
              id = vals[0].var;
              id++;
              console.log('id: ' + id);
              sqlModule.query("UPDATE `globe` SET `var` = '" + id + "' WHERE `globe`.`keyvar` = 1;");
              sqlModule.query("INSERT INTO `meeting` (`id`, `title`, `sponsor`, `start`, `end`, `part`) \
                          VALUES ('"+ id + "', '" + title + "', '" + sponsor + "', '" + start + "', '" + end + "', '" + part + "');");
              console.log('Successful.\n');
              res.send({ status: 'successful' });
            }
            return;
          });
        } else {
          val = vals;
          console.log('Failed. The title has been used.\n');
          res.send({ status: 'failed', err: 'The title has been used!' });
        }
      });
    }
  });
});


app.post('/agenda/deleteMeeting/', (req, res, next) => {
  console.log('Delete Meeting: ' + req.body.mid);
  sqlModule.query("DELETE FROM `meeting` WHERE `meeting`.`id` = '" + req.body.mid + "';");
  res.send({ status: 'successful' });
});


app.post('/agenda/quitMeeting/', (req, res, next) => {
  console.log('Quit Meeting: ' + req.body.mid + req.body.name);
  sqlModule.query("SELECT * FROM `meeting` WHERE `meeting`.`id` = '" + req.body.mid + "';", (vals, isNull) => {
    if (isNull) {
      console.log('Failed.');
      res.send({ status: 'failed' });
    } else {
      var arr = vals[0].part.split(',');
      var flag = false;
      var part = '';
      for (var i of arr) {
        if (i == req.body.name) {
          continue;
        }
        if (flag) {
          part += ',';
        }
        part += i;
        flag = true;
      }
      sqlModule.query("UPDATE `meeting` SET `part` = '" + part + "' WHERE `meeting`.`id` = '" + req.body.mid + "';");
      console.log('Successful.');
      res.send({ status: 'successful' });
    }
  });
});


app.post('/agenda/addMeeting/', (req, res, next) => {
  console.log('Add Meeting: ' + req.body.mid + req.body.name);
  sqlModule.query("SELECT * FROM `meeting` WHERE `meeting`.`id` = '" + req.body.mid + "';", (val, isNull) => {
    if (isNull) {
      res.send({ status: 'failed' });
    } else {
      var start = new Date(val[0].start);
      var end = new Date(val[0].end);
      sqlModule.query("SELECT * FROM `meeting` WHERE `start` < '" + end.toLocaleString() + "' AND `end` > '" + start.toLocaleString() + "';", (vals, isNull) => {
        var flag = true;
        var test_part = '';
        if (isNull) {
          flag = true;
        } else {
          for (var i in vals) {
            if (i == 0) {
              test_part += vals[i].part;
            } else {
              test_part += ',';
              test_part += vals[i].part;
            }
            test_part += ',';
            test_part += vals[i].sponsor;
          }
          var test_arr = test_part.split(',');
          for (var j of test_arr) {
            if (req.body.name == j) {
              flag = false;
              break;
            }
          }
        }
        if (flag === false) {
          res.send({ status: 'failed', err: 'some users have no time' });
        } else {
          sqlModule.query("UPDATE `meeting` SET `part` = '" + val[0].part + "," + req.body.name + "' WHERE `meeting`.`id` = '" + req.body.mid + "';")
          res.send({ status: 'successful' });
        }
      });
    }
  });
});


app.get('/', function (req, res) {
  res.send('Hello World!');
});

var server = app.listen(30002, function () {
  var host = server.address().address;
  var port = server.address().port;
  console.log('Example app listening at http://%s:%s', host, port);
});
