(function(i) {
	try {
		data = JSON.parse(i);
		temperature = data.temperature.toFixed(1);
		return temperature + "ºC";
	}
	catch (err) {
		return "-";
	}
})(input)
