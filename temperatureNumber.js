(function(i) {
	try {
		data = JSON.parse(i);
		temperature = data.temperature.toFixed(1);
		return temperature;
	}
	catch (err) {
		return 0;
	}
})(input)
