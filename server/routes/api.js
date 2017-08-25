const express = require('express');
const router = express.Router();
const url = require('url');

// declare axios for making http requests
/*const axios = require('axios');
const API = 'https://jsonplaceholder.typicode.com';*/

/* GET api listing. */
router.get('/', (req, res) => {
  res.send('api works');
});

var readings = {
    "left": 0,
    "center": 0,
    "right": 0,
}
var url_parts;
var param;
var activeState = "stop";
var activeStateSave = "stop";

// Get all posts
router.get('/posts', (req, res) => {
  // Get posts from the mock api
  // This should ideally be replaced with a service that connects to MongoDB
  axios.get(`${API}/posts`)
    .then(posts => {
      res.status(200).json(posts.data);
    })
    .catch(error => {
      res.status(500).send(error)
    });
});

router.get('/store', (req, res) => {              //stores the incoming readings from the arduino
  var url_parts = url.parse(req.url, true);
	var param = url_parts.query;

	if (param.left != undefined){
	  readings.left = param.left;
	}
	if (param.center != undefined){
	  readings.center = param.center;
	}
	if (param.right != undefined){
    readings.right = param.right
	}
  console.log(readings);
  if (this.activeStateSave != this.activeState){
    this.activeStateSave = this.activeState
    res.status(200).send(this.activeState);
  }else{
    res.status(200).send("");
  }
});

// Get all posts
router.get('/collect', (req, res) => {            //sends the readings back to the frontend
  if (Object.keys(req.query).length !== 0){
    if (req.query['carState']=='true'){
      this.activeState = 'work';
    }else{
      this.activeState = 'stop';
    }
  }
  process.stdout.write(".");
  res.status(200).send(readings);
});

module.exports = router;
