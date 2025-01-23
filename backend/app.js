const db = require('./database');
const cardAccountRouter = require('./routes/card_account');
const transactionRoutes = require('./routes/transaction');
const accountRouter = require('./routes/account');
const customerRouter = require('./routes/customer');

db.getConnection((err, connection) => {
  if (err) {
    console.error('Database connection failed:', err);
  } else {
    console.log('Database connection successful.');
    connection.release();
  }
});

var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var cardRouter = require('./routes/card');

var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/card', cardRouter);
app.use('/card_account', cardAccountRouter);
app.use('/transaction', transactionRoutes);
app.use('/account', accountRouter);
app.use('/customer', customerRouter);

module.exports = app;
