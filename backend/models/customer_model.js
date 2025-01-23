const db = require('../database');

const customer = {
  getAll: function (callback) {
    return db.query('SELECT * FROM customer', callback);
  },

  getById: function (id, callback) {
    return db.query('SELECT * FROM customer WHERE idcustomer = ?', [id], callback);
  },

  add: function (customerData, callback) {
    const { fname, lname, thumbnail } = customerData; // `created_at` is typically auto-handled by the database
    return db.query(
      'INSERT INTO customer (fname, lname, thumbnail) VALUES (?, ?, ?)',
      [fname, lname, thumbnail],
      callback
    );
  },

  update: function (id, customerData, callback) {
    const { fname, lname, thumbnail } = customerData;
    return db.query(
      'UPDATE customer SET fname = ?, lname = ?, thumbnail = ? WHERE idcustomer = ?',
      [fname, lname, thumbnail, id],
      callback
    );
  },

  delete: function (id, callback) {
    return db.query('DELETE FROM customer WHERE idcustomer = ?', [id], callback);
  }
};

module.exports = customer;
