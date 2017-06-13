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
  var email = sqlModule.dealEscape(req.body.email);
  var password = sqlModule.dealEscape(req.body.password);
  console.log('Event: user sign up');
  console.log('username: ' + username);
  console.log('email: ' + email);
  console.log('password: ' + password);
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
                          VALUES ('"+ id + "', '" + username + "', '" + email + "', '" + password + "', '');");
          console.log('succeed.');
          res.send({ status: 'success', id: id });
        }
      });
    } else {
      console.log('Sign up failed, the username is used.');
      res.send({ status: 'failed' });
    }
  });
});

// 登录模块
app.post('/agenda/login', (req, res, next) => {
  var id;
  var username = sqlModule.dealEscape(req.body.username);
  var password = sqlModule.dealEscape(req.body.password);
  console.log('Event: user login');
  console.log('user: ' + username);
  console.log('password: ' + password + '\n');
  sqlModule.query("SELECT * FROM `user` WHERE `name` = '" + username + "';", (vals, isNull) => {
    if (isNull) {
      console.log('User don\'t exist.');
      res.send({ status: 'failed', err: 'User don\'t exist.' });
    } else {
      if (vals[0].password == password) {
        id = vals[0].id;
        console.log('Log in.');
        res.send({ status: 'success', id: id });
      } else {
        console.log('Password error.');
        res.send({ status: 'failed', err: 'Password error.' });
      }
    }
  });
});

//modify
app.post('/agenda/modify', (req, res, next) => {
  var id = sqlModule.dealEscape(req.body.id);
  var email = sqlModule.dealEscape(req.body.email);
  var phone = sqlModule.dealEscape(req.body.phone);
  console.log('Event: user modify');
  console.log('id: ' + id);
  console.log('email: ' + email);
  console.log('phone: ' + phone + '\n');
  sqlModule.query("UPDATE `user` SET `email` = '" + email + "', `phone` = '" + phone + "' WHERE `user`.`id` = " + id + ";");
  res.send({});
});


app.get('/', function (req, res) {
  res.send('Hello World!');
});

var server = app.listen(30002, function () {
  var host = server.address().address;
  var port = server.address().port;
  console.log('Example app listening at http://%s:%s', host, port);
});
