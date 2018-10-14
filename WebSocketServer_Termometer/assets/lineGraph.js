var ctx = document.getElementById("containerLine").getContext('2d');

var temperature = new Chart(ctx, {
	type: 'line',
	data: {
		labels: [],
		datasets: [{
			// backgroundColor: "#dcdcdc",
			borderCapStyle: 'round',
			borderColor: '#808080',
			// borderDash: [5, 5],
			borderWidth: 1,
			data: [],
			fill: false,
			label: 'Temperature',
			steppedLine: false,
			pointRadius: 0
		}]
	},
	options: {
		responsive: true,
		legend: {
			display: true,
		},
		hover: {
			mode: 'label'
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: true,
					labelString: 'Time'
				}
			}],

			yAxes: [{
				display: true,
				scaleLabel: {
					display: true,
					labelString: 'Temp. °C'
				},
				ticks: {
					suggestedMin: 15,
					suggestedMax: 60,
					stepSize: 5,
					beginAtZero: false
				}
			}]
		},
		title: {
			display: true,
			text: 'Cabudare - Lara'
		}
	}
});