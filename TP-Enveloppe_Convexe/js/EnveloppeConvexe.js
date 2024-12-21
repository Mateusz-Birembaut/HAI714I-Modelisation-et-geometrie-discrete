const algoEnvConv = {demiPlan: 0, jarvis:1, graham:2};

class EnveloppeConvexe{
	constructor(lesPoints, a){
		this.algo = a ?? algoEnvConv.jarvis;
		this.points = lesPoints;
		this.envconv = new Array();
		this.initEnvConv();
	}

	getElements() {
		let ec = new Array();
		for (let i = 0; i < this.envconv.length; i++) {
			ec.push(this.points[this.envconv[i]]);
		}
		return ec;
	}

	initEnvConv() {
		switch (this.algo) {
			case algoEnvConv.demiPlan:
				this.envconv = this.algoDemiPlan();
				break;
			case algoEnvConv.jarvis:
				this.envconv = this.algoJarvis();
				break;
			case algoEnvConv.graham:
				this.algoGraham();
				break;
			default:
				console.log("Warning: algorithm not defined");
				break;
		}
	}

	tour(a, b, c) {
		return Coord2D.tour(a, b, c);
	}

	findFirst(points, i, j) {
		for (let f = 0; f < points.length; f++) {
				if (f != i && f != j) {
					return f; 
				}
		}
		throw new Error("aucun point different de i et j");
	}
	
	
	algoDemiPlan(){
		console.log("algo d'intersection des demi-plans")	
		let n = this.points.length;
		let current, previous, i, j ,k ,f;
		let mc;
		let result = new Array();

		for (i = 0; i < n; i++) {
			for (j = 0; j < i; j++) {
				mc = true;
				f = this.findFirst(this.points, i, j)
				previous = this.tour(this.points[i], this.points[j], this.points[f]);
				k = f+1
				while (k < n && mc){
					if (k != i && k != j){
						current = this.tour(this.points[i], this.points[j], this.points[k]);
						if (current == 0){
							throw new Error(`Points alignés : (${i}, ${j}, ${k})`);
						}else if (current != previous){
							mc = false;
						}
					}
					previous = current;
					k++;
				}

				if(mc){
					if(current > 0){
						result.push(i);
						result.push(j);
					}
					else if(current < 0){
						result.push(j);
						result.push(i);
					}
					else if(current == 0 && k < n){
						console.log(`Points alignés : (${this.points[i]}, ${this.points[j]}, ${this.points[k]})`);
						throw new Error(`Points alignés : (${i}, ${j}, ${k})`);
					}
					
				}
			}
		}
		return result;
	}

	minY(points) {
		let min_index = 0;
		for (let i = 1; i < points.length; i++) {
			if (points[i].y < points[min_index].y) {
				min_index = i;
			}
		}
		return min_index;
	}

	findNext(idX) {
		let rightmost_pt_index = null;
		let p = this.points[idX];
		for (let i = 0; i < this.points.length; i++) {
			if(i != idX){
				if (rightmost_pt_index == null) {
					rightmost_pt_index = i;
				} else {
					let candidate = this.points[i];
					let tour = this.tour(p, this.points[rightmost_pt_index], candidate);
					if (tour < 0) {
						rightmost_pt_index = i;
					} else if (tour === 0) {
						console.log("3 Points alignés");
						console.log(" p : ", p);
						console.log(" nextPoint ", this.points[rightmost_pt_index]);
						console.log(" candidta ", candidate);
						throw new Error("3 Points alignés");
					}
				}
			}
		}
		return rightmost_pt_index;
	}
	
	algoJarvis() {
		console.log("algo Jarvis");
		let min = this.minY(this.points);
		let result = [min];
		let current = undefined;
		let previous = min;
		while (current != min) {
			current = this.findNext(previous);
			if (current == min) {
				break;
			}
			result.push(current);
			previous = current;
		}

		return result;

	}

	algoGraham(){
		console.log("algo de Graham")
		//todo


	}

	//fonction à appeler pour afficher l'enveloppe convexe dans le canvas
	drawIn(displayer) {
		displayer.drawLineLoop(this.getElements(), ["#FA3", "#41566E", "#FA3" ], true);
	}
}