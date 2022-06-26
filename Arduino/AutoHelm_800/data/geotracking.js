function init_logger(){
  var logger = document.getElementById("logger");
  console.log = function(text) {
      var element = document.createElement("div");
      var txt = document.createTextNode(text);

      element.appendChild(txt);
      logger.appendChild(element);
  }

  // testing
  console.log("Hello World...");
}

function log(text){
  var logger = document.getElementById("logger");
  var element = document.createElement("div");
  var txt = document.createTextNode(text);

  element.appendChild(txt);
  logger.appendChild(element);
}

const nbPositions = 2;
let positions = [];

function geo_error(error){
  log(error.code);
  log(error.message);
}

var interval;
function geo_init() {
  console.log("init geotracing");
  interval = setInterval(function(){
    if(navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(geo_setPosition, geo_error);
    }
  }, 5000);
}

function geo_setPosition(pos) {
  // log("lat:"+pos.coords.latitude+" lon:"+ pos.coords.longitude);
  positions.push(pos);
  if(positions.length<nbPositions)
      return;
  lastPos = positions.shift();
  log("distance:"+ distance(lastPos.coords,pos.coords).toFixed()+", bearing:"+ bearing(lastPos.coords,pos.coords).toFixed());
}

function bearing(pos1, pos2){
    const φ1 = pos1.latitude * Math.PI/180; // φ, λ in radians
    const φ2 = pos2.latitude * Math.PI/180;
    const λ1 = pos1.longitude * Math.PI/180; // φ, λ in radians
    const λ2 = pos2.longitude * Math.PI/180;

    const y = Math.sin(λ2-λ1) * Math.cos(φ2);
    const x = Math.cos(φ1)*Math.sin(φ2) -
          Math.sin(φ1)*Math.cos(φ2)*Math.cos(λ2-λ1);
    const θ = Math.atan2(y, x);
    const brng = (θ*180/Math.PI + 360) % 360; // in degrees
    return brng;
}

function distance(pos1, pos2){
    const R = 6371e3; // metres
    const φ1 = pos1.latitude * Math.PI/180; // φ, λ in radians
    const φ2 = pos2.latitude * Math.PI/180;
    const Δφ = (pos2.latitude-pos1.latitude) * Math.PI/180;
    const Δλ = (pos2.longitude-pos1.longitude) * Math.PI/180;

    const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
            Math.cos(φ1) * Math.cos(φ2) *
            Math.sin(Δλ/2) * Math.sin(Δλ/2);
    const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

    const d = R * c; // in metres
    return d;
}