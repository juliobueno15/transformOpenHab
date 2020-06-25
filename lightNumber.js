(function(i) {
	try {
		data = JSON.parse(i);
		light = data.light.toFixed(0);
		return light;
	}
	catch (err) {
		return 0;
	}
})(input)