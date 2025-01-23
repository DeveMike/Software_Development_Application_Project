const express = require('express');
const router = express.Router();
const customerModel = require('../models/customer_model');

// Get all customers
router.get('/', (req, res) => {
  customerModel.getAll((err, results) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(results);
    }
  });
});

// Get customer by ID
router.get('/:id', (req, res) => {
  const id = req.params.id;
  customerModel.getById(id, (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json(result[0]);
    }
  });
});

// Add a new customer
router.post('/', (req, res) => {
  const newCustomer = req.body;
  customerModel.add(newCustomer, (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.status(201).json({ message: 'Customer added!', id: result.insertId });
    }
  });
});

// Update a customer
router.put('/:id', (req, res) => {
  const id = req.params.id;
  const updatedCustomer = req.body;
  customerModel.update(id, updatedCustomer, (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json({ message: 'Customer updated!' });
    }
  });
});

// Delete a customer
router.delete('/:id', (req, res) => {
  const id = req.params.id;
  customerModel.delete(id, (err, result) => {
    if (err) {
      res.status(500).json({ error: err.message });
    } else {
      res.json({ message: 'Customer deleted!' });
    }
  });
});

module.exports = router;
