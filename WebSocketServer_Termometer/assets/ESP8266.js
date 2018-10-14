'use strict'

const wsURL = "ws://10.0.0.102:8080/",
	websocket = new WebSocket(wsURL);

websocket.onopen = () => onOpen();
websocket.onclose = () => onclose();
websocket.onmessage = (message) => onMessage(message);
websocket.onerror = (err) => onerror(err);

let yData = new Array();
let xData = new Array();


const onOpen = () => websocket.send('Connected since ' + new Date()),

	onClose = () => console.log('Closed at' + new Date()),

	onError = (err) => console.error('WebSocket Error ', err),

	onMessage = (message) => {

		displayTemperature(message.data);

		gaugeGraph(message.data);

		lineGraph(message.data);

	},

	displayTemperature = (temp) => {
		document.getElementById("app").innerHTML = `<h2>${temp} °C</h2>`
	},

	gaugeGraph = (temp) => {
		if (!chartGauge.renderer.forExport) {
			const point = chartGauge.series[0].points[0];
			point.y = parseFloat(temp);
			point.update();
		}
	},

	lineGraph = (temp) => {

		let xValue = moment().utc(-4).format('hh:mm:ss');

		temperature.data.labels.push(xValue);
		temperature.data.datasets.forEach((element) => {
			// Push the array and is the length > 1000, drop the first element ......
			if (element.data.push(temp) >= 1000) {
				element.data.shift();
				temperature.data.labels.shift();
			}
		});

		temperature.update();
	};


			//************************************HighChars******************************************** */

			// if (yData.push(message.data) >= 50) yData.shift();
			// xData.push(xValue);

			// yData = yData.map(element => parseFloat(element));
			// chartTemperature.series[0].setData(yData);
			// chartTemperature.series[0].setData(xData);

			//*************************************GAUGE********************************************** */