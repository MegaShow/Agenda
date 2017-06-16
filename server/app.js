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
  sqlModule.query("SELECT * FROM `user` WHERE `name` = '" + username + "';", (vals, isNull) => {
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
  sqlModule.query("SELECT * FROM `user` WHERE `name` = '" + username + "';", (vals, isNull) => {
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
  console.log('ID: ' + id);
  sqlModule.query("SELECT * FROM `user` WHERE `id` = " + id + ";", (vals, isNull) => {
    if (isNull) {
      console.log('Failed.\n');
      res.send({ status: 'failed' });
    } else {
      _email = vals[0].email;
      _phone = vals[0].phone;
      console.log('Successful.\n');
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

// 获取会议信息
app.post('/agenda/meeting', (req, res, next) => {
  console.log('Event: Get Meeting List');
  sqlModule.query("SELECT * FROM `meeting`;", (vals, isNull) => {
    if (isNull) {
      console.log('No meeting\n');
      res.send({ status: 'failed'});
    } else {
      console.log('Success\n');
      res.send({ status: 'successful', meeting: vals });
    }
  })
});


// 获取会议信息
app.post('/agenda/user', (req, res, next) => {
  console.log('Event: Get User List');
  sqlModule.query("SELECT * FROM `user`;", (vals, isNull) => {
    if (isNull) {
      console.log('No user\n');
      res.send({ status: 'failed'});
    } else {
      console.log('Success\n');
      res.send({ status: 'successful', user: vals });
    }
  })
});


app.get('/', function (req, res) {
  res.send('Hello World!');
});

var server = app.listen(30002, function () {
  var host = server.address().address;
  var port = server.address().port;
  console.log('Example app listening at http://%s:%s', host, port);
});
