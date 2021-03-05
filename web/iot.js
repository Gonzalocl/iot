let main_objet = document.getElementById('main_objet');
main_objet.innerHTML = 'Loading...';
let data_request = new XMLHttpRequest();
data_request.responseType = 'json';
data_request.open('GET', '/data', true);
data_request.onload = data_received;
data_request.send(null);

function data_received() {
    let sensor_data = data_request.response;
    create_history_table(sensor_data);
    create_fan_button(sensor_data);
}

function create_history_table(sensor_data) {
    let history = document.createElement('table');
    add_row(history, 'Temperature history', 'Humidity history');
    for (let i = 0; i < sensor_data.temperature_history.length; i++) {
        add_row(history, sensor_data.temperature_history[i], sensor_data.humidity_history[i])
    }
    main_objet.innerHTML = '';
    main_objet.appendChild(history);
}

function add_row(history, temperature, humidity) {
    let row = history.insertRow();
    let temperature_cell = row.insertCell(0);
    let humidity_cell = row.insertCell(1);
    temperature_cell.innerHTML = temperature;
    humidity_cell.innerHTML = humidity;
}

function create_fan_button(sensor_data) {
    let fan_state = document.createElement('div');
    let fan_switch = document.createElement('button');
    if (sensor_data.fan_on === '0') {
        fan_state.textContent = 'Fan state: OFF';
        fan_switch.textContent = 'ON';
    } else {
        fan_state.textContent = 'Fan state: ON';
        fan_switch.textContent = 'OFF';
    }
    main_objet.appendChild(fan_state);
    main_objet.appendChild(fan_switch);
}
