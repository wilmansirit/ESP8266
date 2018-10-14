const chartTemperature = Highcharts.chart('container', {
	chart: {
		type: 'line'
	},
	title: {
		text: 'Monthly Average Temperature'
	},
	subtitle: {
		text: 'Source: WorldClimate.com'
	},
	xAxis: {
		categories: []
	},
	yAxis: {
		title: {
			text: 'Temperature (°C)'
		}
	},
	plotOptions: {
		line: {
			dataLabels: {
				enabled: true
			},
			enableMouseTracking: false
		}
	},
	series: [{
		name: 'Date',
		data: []
	}]
});