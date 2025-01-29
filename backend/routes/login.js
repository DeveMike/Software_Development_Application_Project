const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const card = require('../models/card_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');

router.post('/', function(request, response) {
  if (request.body.idcard && request.body.pin) {
    const cardID = request.body.idcard;
    const cardPin = request.body.pin;

    card.checkPin(cardID, function(dbError, dbResult) {
      if (dbError) {
        response.send("!404!");
      } else {
        if (dbResult.length > 0) {
          bcrypt.compare(cardPin, dbResult[0].pin, function(err, compareResult) {
            if (compareResult) {
              console.log("Success");
              const token = generateAccessToken({ idcard: cardID });
              response.send(token);
            } else {
              console.log("Wrong PIN");
              response.send(false);
            }
          });
        } else {
          console.log("Card does not exist");
          response.send(false);
        }
      }
    });
  } else {
    console.log("Card ID or PIN missing");
    response.send(false);
  }
});

function generateAccessToken(idcard) {
  dotenv.config();
  return jwt.sign(idcard, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;