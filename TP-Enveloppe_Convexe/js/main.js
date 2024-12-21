function initAndRender() {
    const canvas = document.getElementById("canvas"),
        displayer = new Displayer(canvas),
        colors = ["#FEE", "#FFF", "#41566E"],
        nbSommets = 25;
    let algo = getAlgo((document.querySelector("input[type=radio]:checked")).id);

    function getAlgo(id) {
        return (id == "ri0") ? algoEnvConv.demiPlan : (id == "ri1") ? algoEnvConv.jarvis : algoEnvConv.graham;
    }

    let points;
    points = (new Dataset()).initRandomPoints(nbSommets, canvas.width, canvas.height, "p");
    //	let points = (new Dataset()).initFixedPoints("p");	
    displayer.drawPoints(points, colors, false);
    const coord2D = new Coord2D();

    function initAndDrawEnvConv() {
        let envconv = new EnveloppeConvexe(points, algo);
        displayer.drawPoints(points, colors, false);
        envconv.drawIn(displayer);
    }

    document.getElementById("choixAlgo").addEventListener("click", function (e) {
        let id = e.target.id;
        algo = getAlgo(id);
        initAndRender();
    });
    document.getElementById("canvas").addEventListener("click", initAndRender);
    initAndDrawEnvConv();
    
    //displayer.drawLines(points, colors, false);
    
    /*
    function testTour(){
        const coord2D = new Coord2D();


        const p1 = new Point(0, 0, "p1");
        const p2 = new Point(1, 1, "p2");
        const p3 = new Point(1, 2,  "p3");

        console.log(aop);

        const result = Coord2D.tour(p1, p2, p3);
        console.log(result);
    
        const p4 = new Point(1, 0, "p4");
        const p5 = new Point(1, 1, "p5");
        const p6 = new Point(0, 1, "p6");
    
        const result2 = Coord2D.tour(p4, p5, p6);
        console.log(result2);
    
        const result3 = Coord2D.tour(p6, p5, p4);
        console.log(result3);
    };
    
    //testTour();
    */

}