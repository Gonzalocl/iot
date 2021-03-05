document.getElementById('main_objet').innerHTML = 'Loading...';
var data_request = new XMLHttpRequest();
data_request.responseType = 'json';
data_request.open('GET', '/data', true);
data_request.onload = data_received;
data_request.send(null);

function data_received() {
    var sensor_data = data_request.response;
    console.log(sensor_data);
}
