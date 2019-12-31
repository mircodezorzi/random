function onMouseDrag(event) {
	paper.view.center += event.downPoint - event.point;
}

view.element.onmousewheel = function(event) {
	view.zoom += event.wheelDeltaY / 1200;
};

function bin2dec(bits) {
	return parseInt(bits.join(''), 2);
}

function line(x1, y1, x2, y2) {
	var path = new Path();
	path.strokeColor = 'black';
	path.add(new Point(x1, y1));
	path.add(new Point(x2, y2));
}

function carrier(x) {
	return Math.sin(x / (50 / Math.PI)) * 50;
}

function plot(name, x, y, w, h, func) {
	line(x, y + h / 2, x + w, y + h / 2);
	line(x, y, x, y + h);

	var text = new PointText(view.center);
	text.content = name;
	text.setPosition(x, y + h / 2);
	text.style = {
		fontFamily: 'Monospace',
		fontSize: 20,
		justification: 'right'
	};

	var path = new Path();
	path.strokeColor = 'black';

	var s = 100;
	for (var i = s; i < w; i += s)
		line(x + i, y - h * 0.1, x + i, y + h * 1.1);

	for (var i = 0; i < w; i++)
		func(i, path);
}

function plot_func(name, x, y, w, h, func) {
	plot(name, x, y, w, h, function(i, path) {
		path.add(x + i, y + h / 2 + func(i));
	});
}

function plot_bits(x, y, w, h, bits) {
	var last = 0;
	plot('binary data', x, y, w, h, function(i, path) {
		bit = bits[Math.floor(i / 100)];
		path.add(x + i + (last == bit), y + h / 2 + (bit ? -1 : 1) * h / 2);
		last = bit;
	});
}

function differential_manchester(x, y, w, h, bits) {
	var last = 0;
	plot('dmanch', x, y, w, h, function(i, path) {
		bit = bits[Math.floor((i + 50) / 100)];
		path.add(x + i + (last == bit), y + h / 2 + (bit ? -1 : 1) * h / 2);
		last = bit;
	});
}

function manchester(x, y, w, h, bits) {
	line(x, y + h / 2, x + w, y + h / 2);
	line(x, y, x, y + h);

	var text = new PointText(view.center);
	text.content = 'manch';
	text.setPosition(x, y + h / 2);
	text.style = {
		fontFamily: 'Monospace',
		fontSize: 20,
		justification: 'right'
	}

	var path = new Path();
	path.strokeColor = 'black';

	var s = 100;
	for (var i = s; i < w; i += s)
		line(x + i, y - h * 0.1, x + i, y + h * 1.1);

	for (var i = 0; i < bits.length; i++) {
		if (bits[i]) {
			path.add(x + (i * 100), y);
			path.add(x + ((2 * i + 1) * 50), y);
			path.add(x + ((2 * i + 1) * 50), y + 100);
			path.add(x + ((i + 1) * 100), y + 100);
		} else {
			path.add(x + (i * 100), y + 100);
			path.add(x + ((2 * i + 1) * 50), y + 100);
			path.add(x + ((2 * i + 1) * 50), y);
			path.add(x + ((i + 1) * 100), y);
		}
	}

}

function ask(x, y, w, h, d, f) {
	plot_func('ask', x, y, w, h, function(x) {
		return (d[Math.floor(x / w * d.length)] == 1 ? 1 : 0.5) * f(x);
	});
}

function psk(x, y, w, h, d, f) {
	plot_func('psk', x, y, w, h, function(x) {
		return (d[Math.floor(x / w * d.length)] == 1 ? 1 : -1) * f(x);
	});
}

function fsk(x, y, w, h, d, f) {
	plot_func('fsk', x, y, w, h, function(x) {
		return f(x * (d[Math.floor(x / w * d.length)] == 1 ? 3/2 : 2/3));
	});
}

function qam(x, y, w, h, bits, f, l) {
	line(x, y + h / 2, x + w, y + h / 2);
	line(x, y, x, y + h);

	var text = new PointText(view.center);
	text.content = 'qam';
	text.setPosition(x, y + h / 2);
	text.style = {
		fontFamily: 'Monospace',
		fontSize: 20,
		justification: 'right'
	};

	var path = new Path();
	path.strokeColor = 'black';

	var s = 100;
	for (var i = s; i < w; i += s)
		line(x + i, y - h * 0.1, x + i, y + h * 1.1);

	chunks = []
	for (var i = 0; i < bits.length; i += l) {
		chunks.push(bin2dec(bits.slice(i, i + l)))
	}

	for (var i = 0; i < 200; i++) {
		path.add(x + i, y + h / 2 + ((chunks[Math.floor(i / 100)] / (l * l)) * f(i)));
	}
}

var bits = [0, 1, 0, 1, 1, 0, 1, 1];
plot_bits(100, 50, 800, 100, bits);
plot_func('carrier', 100, 200, 800, 100, carrier);
ask(100, 350, 800, 100, bits, carrier);
psk(100, 500, 800, 100, bits, carrier);
fsk(100, 650, 800, 100, bits, carrier);
qam(100, 800, 800, 100, bits, carrier, 4);
differential_manchester(100, 950, 800, 100, bits);
manchester(100, 1100, 800, 100, bits);
