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
          res.send({ status: 'failed'});
        } else {
          id = vals[0].var;
          id++;
          console.log('id: ' + id);
          sqlModule.query("UPDATE `globe` SET `var` = '" + id + "' WHERE `globe`.`keyvar` = 0;");
          sqlModule.query("INSERT INTO `user` (`id`, `name`, `email`, `password`, `phone`) \
                          VALUES ('"+id+"', '"+username+"', '"+email+"', '"+password+"', '');");
          console.log('succeed.');
          res.send({ status: 'success', id: id});
        }
      });
    } else {
      console.log('Sign up failed, the username is used.');
      res.send({ status: 'failed' });
    }
  });
});

// 登录模块
app.post('/login', (req, res, next) => {
  console.log('Event: user login');
  console.log(req.body.username);
  res.send({ status: 'success' });
});

//用户登录模块
/*app.post('/logintest', [userModule.isEmailStr], (req, res, next) =>  { //用户是否存在
  console.log('User Login:');
  var sqlCmd = 'SELECT `id`, `name`, `password`, `detail`, `web`, `tureEmail` FROM `user` WHERE `email`=\'' +
  req.body.userEmail + '\'';
  sqlModule.query(sqlCmd, (vals, isNull) =>  {
    if (isNull) {
      console.log('ERR: user is not exist.');
      res.send({ state: 'failed', why: 'ERROR_USER' });
      next('route');
    } else {
      res.locals.userData = vals[0];
      next();
    }
  });
}, (req, res, next) =>  { //密码是否正确
  if (res.locals.userData.password == userModule.makeAsha(req.body.userPassword)) {
    var newToken = Math.round(Math.random() * 10000000);
    userModule.getToken(res.locals.userData.id, newToken, (oldToken,tureEmail) => {
      var nowTime = new Date().Format('yyyy-MM-dd hh:mm:ss');
      res.locals.data = {//构建session原始数据
        userID: res.locals.userData.id,
        token: newToken,
        lastDate: nowTime
      };
      userModule.makeASign(req, res, () =>  {
        res.send({
          state: 'success',
          name: res.locals.userData.name,
          detail: res.locals.userData.detail,
          web: res.locals.userData.web,
          tureEmail: res.locals.userData.tureEmail
        });
      });
    });
  } else {
    console.log('ERR: password is error');
    res.send({ state: 'failed', why: 'ERROR_PASSWORD' });
    next('route');
  }
});*/

app.get('/', function (req, res) {
  res.send('Hello World!');
});

var server = app.listen(30002, function () {
  var host = server.address().address;
  var port = server.address().port;
  console.log('Example app listening at http://%s:%s', host, port);
});
