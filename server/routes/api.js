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
    "left": 30,
    "center": 60,
    "right": 90,
}

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

router.get('/store', (req, res) => {
 /* readings.left = req.body.left
  readings.center = req.body.center
  readings.right = req.body.right
  console.log(req);
  //console.log("Received measurement: ");
  console.log("Left:"+readings.left +" Center:"+ readings.center +" Right:"+ readings.right);
*/

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
  res.status(200).send("MESSAGE");
});

// Get all posts
router.get('/collect', (req, res) => {
  //console.log(readings);
  process.stdout.write(".");
  res.status(200).send(readings);
});

module.exports = router;
