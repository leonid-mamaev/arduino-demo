var http = require('http');
var sys = require('sys');
var exec = require('child_process').exec;
var url = require('url');
var http = require('http');

http.createServer(function(request, response) {
  response.writeHead(200, {"Content-Type": "text/html; charset=utf-8"});
  exec("bash drupal-notify.sh;", puts);
  console.log("Arduino works!");
  response.end();
}).listen(port);


function puts(error, stdout, stderr) { 
  sys.puts("Node created");
  //sys.puts(stdout);
  //sys.puts(stderr);
}
