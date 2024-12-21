class Point {
    // représentation d'un point cartésiennes 
    constructor(x, y, label) {
        this.label = label ?? "p";
        this.x = x;
        this.y = y;
    }
    printCoords() {
        console.log(this.x.toPrecision(2) + "," + this.y.toPrecision(2));
    }

}

class Displayer {
    constructor(canvas, minX, maxX, minY, maxY, colors) {
    	this.canvas = canvas;
        if (canvas.getContext) {
            this.g2d = canvas.getContext('2d');
        } else {
            canvas.write("Votre navigateur ne peut visualiser cette page correctement");
        }

       this.lineWidth = 1;

        this.setColors(colors);

        //dimensions du display
        this.cWidth = this.canvas.width;
        this.cHeight = this.canvas.height;
        this.cCenterX = this.canvas.width / 2;
        this.cCenterY = this.canvas.height / 2;

        //dimensions des données à afficher
        this.minX = minX ?? 0;
        this.maxX = maxX ?? this.canvas.width;
        this.minY = minX ?? 0;
        this.maxY = maxX ?? this.canvas.height;

        this.init();
    }

    setColors(colors){
    	this.colors = {fg:"#FEE",txt:"#FFF",bg:"#999"};
    	if (colors) {
        	this.colors.fg = colors[0] ?? this.colors.fg;
        	this.colors.txt = colors[1] ?? this.colors.txt;
        	this.colors.bg = colors[2] ?? this.colors.bg;
        }
    }

    init() {
        this.g2d.clearRect(0, 0, this.cWidth, this.cHeight);
        this.g2d.fillStyle = this.colors.bg;
        this.g2d.fillRect(0, 0, this.cWidth, this.cHeight);
     }

    xm2p(x) {
        return this.cWidth * (x - this.minX) / (this.maxX - this.minX);
    }

    ym2p(y) {
        return this.cHeight * (y - this.maxY) / (this.minY - this.maxY);
    }

    //dessin point donné par coordonnées cartésiennes (transformées en coordonnées canvas)
    mDrawPoint(p) {
        this.g2d.beginPath();
        let x = this.xm2p(p.x),
            y = this.ym2p(p.y);
        
        this.g2d.arc(x, y, 15, 0, Math.PI * 2, true);
        this.g2d.fillStyle = this.colors.bg;
        this.g2d.fill();
        this.g2d.strokeStyle = this.colors.fg;
        this.g2d.stroke();

        this.g2d.fillStyle = this.colors.txt;
        let dx = p.label.length * 3,
            dy = 2;
        this.g2d.fillText(p.label, x - dx, y + dy);
    }

    //dessin segment entre points en coordonnées cartésiennes
    mDrawLine(p1, p2) {
        this.g2d.strokeStyle = this.colors.fg;
        this.g2d.beginPath();
        this.g2d.moveTo(this.xm2p(p1.x), this.ym2p(p1.y));
        this.g2d.lineWidth = this.lineWidth;
        this.g2d.lineJoin = "round";
        this.g2d.lineTo(this.xm2p(p2.x), this.ym2p(p2.y));
        this.g2d.stroke();
    }

    //dessin segment entre points en coordonnées "canvas" 
    pDrawLine(x1, y1, x2, y2) {
        this.g2d.strokeStyle = this.colors.fg;
        this.g2d.beginPath();
        this.g2d.moveTo(x1, y1);
        this.g2d.lineTo(x2, y2);
        this.g2d.stroke();
    }

    setOptions(points, colors, incrementalDrawing) {
    	this.points = points ?? this.points;
     	this.incrementalDrawing = incrementalDrawing ?? false;
        this.setColors(colors);
        if(!incrementalDrawing) this.init();
    }

    drawPoints(points, colors, incrementalDrawing) {
     	this.setOptions(points, colors, incrementalDrawing);
       	let n = points.length;
        for (let i = 0; i < n; i++) {
        	this.mDrawPoint(points[i], colors);
        }
    }

    drawLineLoop(points, colors, incrementalDrawing) {
     	this.setOptions(points, colors, incrementalDrawing);
        let n = points.length;
        if (n == 0) return;
        for (let i = 0; i < n; i++) {
            this.mDrawLine(points[i], points[(i + 1) % n], colors);
            this.mDrawPoint(points[i], colors);
        }
        this.mDrawPoint(points[0], colors);
    }
    /*
    drawLines(points, colors, incrementalDrawing) {
     	this.setOptions(points, colors, incrementalDrawing);
        let n = points.length;
        if (n == 0) return;
        for (let i = 0; i < n - 1; i += 2) {
            this.mDrawLine(points[i], points[(i + 1) % n], colors);
            this.mDrawPoint(points[i], colors);
            this.mDrawPoint(points[i + 1], colors);
        }
    }
    */

    drawLines(points, colors, incrementalDrawing) {
        this.setOptions(points, colors, incrementalDrawing);
        let n = points.length;
        if (n == 0) return;

        let coord2D = new Coord2D();

        for (let i = 0; i < n - 2; i = i+2) {
            let p1 = points[i];
            let p2 = points[(i+1)];
            let p3 = points[(i + 2)];

            let turnType = Coord2D.tour(p1, p2, p3);

            let color;
            if (turnType > 0) {
                color = '#F00'; // Direct 
            } else if (turnType < 0) {
                color = '#0F0'; // Indirect 
            } else {
                color = '#000'; // Aligned
            }

            this.mDrawLineWithColor(p1, p2, color);
            this.mDrawLineWithColor(p2, p3, color);
            this.mDrawPoint(p1, color);
            this.mDrawPoint(p2, color);
            this.mDrawPoint(p3, color);
        }
    }

    mDrawLineWithColor(p1, p2, color) {
        this.g2d.strokeStyle = color;
        this.g2d.beginPath();
        this.g2d.moveTo(this.xm2p(p1.x), this.ym2p(p1.y));
        this.g2d.lineWidth = this.lineWidth;
        this.g2d.lineJoin = "round";
        this.g2d.lineTo(this.xm2p(p2.x), this.ym2p(p2.y));
        this.g2d.stroke();
    }

};

class Coord2D{
    static determinant(u, v){
        return u.x * v.y - u.y * v.x;
    }

    static detSign(u, v){
        let det = this.determinant(u, v);
        if (det > 0) return 1;
        if (det < 0) return -1;
        return 0;
    }

    static tour(p1, p2, p3){
        let u = { x: p2.x - p1.x, y: p2.y - p1.y };
        let v = { x: p3.x - p1.x, y: p3.y - p1.y };
        let detSign = this.detSign(u, v);
        if (detSign > 0) return 1;
        if (detSign < 0) return -1;
        return 0;
    }

}
