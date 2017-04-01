// --------------------------------------------- //
// ------- Random Game Kristofer Brink --------- //
// -------- Created by Nikhil Suresh ----------- //
// -------- Three.JS is by Mr. doob  ----------- //
// --------------------------------------------- //


// ------------------------------------- //
// ------- GLOBAL VARIABLES ------------ //
// ------------------------------------- //

// scene object variables
var renderer, scene, camera;
var playerBall;
var cameraPos = new THREE.Vector3(0,0,0);
var avalanche = [];
var wallHeight = 3000 / 2;
var wallWidth = 136 / 2;
var chancePerFrame = 50;

var score = 0;
var scoreElem;
jQuery(document).ready(function () {scoreElem = jQuery('#score');});

function deltaScore(x) {
    score += x;
    if (scoreElem)
        scoreElem.text(String(score));
}


var myLoadImage, myLoadTexture;
(function (){
    var imageLoader = new THREE.ImageLoader();
    myLoadImage = function () {
        arguments[0] = arguments[0];
        return imageLoader.load.apply(imageLoader, arguments);
    };
    
    var textureLoader = new THREE.TextureLoader();
    textureLoader.crossOrigin = 'anonymous';
    myLoadTexture = function () {
        arguments[0] = arguments[0];
        return textureLoader.load.apply(textureLoader, arguments);
    };
})();

// ------------------------------------- //
// ------- GAME FUNCTIONS -------------- //
// ------------------------------------- //

function getImages(){
    return jQuery.ajax('https://api.imgur.com/3/gallery/random/random', {
  headers: {
    Accept: 'application/json',
    Authorization: 'Client-ID 6a5400948b3b376'
  },
  type: 'GET'
}).then(function (data) {
  return Promise.all(jQuery.map(data.data, function (image) {
    //console.log('Considering ' + image.link);
    if (image.nsfw !== false)
      return;
    if (image.animated)
      return;
    return new Promise(function (resolve, reject) {
        myLoadTexture(/(.*)\.([^.]*)$/.exec(image.link).slice(1).join('l.'), resolve, /* progress */undefined, function () {
            resolve();
        });
    });
  })).then(function (possiblyLoadedTextures) {
      // Filter out unloadable.
      return jQuery.map(possiblyLoadedTextures, function (texture) { return texture; });
  });
}, function () {
console.log('Fail image loader');
});
}
var images;

function setup()
{
    var sciframe;
    jQuery('#gameCanvas').before(
        sciframe = jQuery('<iframe/>', {
            style: 'position: absolute; display: none;',
            width: 300,
            height: 120,
            scrolling: 'no',
            frameborder: 'no',
            src: 'https://w.soundcloud.com/player/?url=https%3A//api.soundcloud.com/users/39687161/favorites&auto_play=true&hide_related=false&show_comments=true&show_user=true&show_reposts=false&visual=true&start_track=' + (Math.random() * 20 | 0)
            }));
	
    // Set up physics
	Physijs.scripts.worker = 'Scripts/physijs_worker.js';
    Physijs.scripts.ammo = 'ammo.js';
    
    images = getImages();

    // set up all the 3D objects in the scene	
	createScene().then(function () {
	
	   // and let's get cracking!
	   draw();
    });
}

function setSceneBasics(){
    // set the scene size
	var WIDTH = window.innerWidth;
	var HEIGHT = window.innerHeight;

	var c = document.getElementById("gameCanvas");

	// create a WebGL renderer, camera
	// and a scene
	renderer = new THREE.WebGLRenderer({antialias: true, logarithmicDepthBuffer: true});
    renderer.shadowMap.enabled = true;
    
    // Make camera
	camera =
	  new THREE.PerspectiveCamera(
		15, // View angle
		WIDTH / HEIGHT, // Aspect
		0.1, // Near
		3000000); // Far
    // rotate to face towards the opponent
    camera.rotation.x = -30 * Math.PI/180;
	camera.rotation.y = -30 * Math.PI/180;
    
    scene = new Physijs.Scene();
	
    // add the camera to the scene
	scene.add(camera);
	
	// set a default position for the camera
	// not doing this somehow messes up shadow rendering well this doesn't do anything says Kristofer Brink
	//camera.position.z = 200;
	
	// start the renderer
	renderer.setSize(WIDTH, HEIGHT);

	// attach the render-supplied DOM element
	c.appendChild(renderer.domElement);
}

function setGravity() {
    scene.setGravity(new THREE.Vector3( 0, -wallHeight/50, 0 ));
}

function makePlayerBox(){
     // ------------------------------------------- playerBall
	// // create the playerBall's material
    
    // Materials
    return images.then(function (images) {
        var playerBallMaterial = Physijs.createMaterial(
            new THREE.MeshLambertMaterial({ map: images[(Math.random() * images.length - 0.01)|0], transparent: true, opacity: .9 }),
            .8, // high friction
            .5 // low restitution
        );
        playerBallMaterial.map.wrapS = playerBallMaterial.map.wrapT = THREE.RepeatWrapping;
        playerBallMaterial.map.repeat.set( 1, 1 );


        playerBall = new Physijs.BoxMesh( 

           new THREE.SphereGeometry( 
              wallWidth/ 12, 
              100,
              100), 
            playerBallMaterial,
            5); // Gravity

        playerBall.receiveShadow = true;
        playerBall.castShadow = true;
        playerBall.position.set(0,0,0);

        // // add the sphere to the scene
        scene.add(playerBall);
        // Slow down the ball
        playerBall.setDamping( .999, .97);

        playerBall.setAngularFactor(new THREE.Vector3(2,2,2));

        playerBall.rotation.set(new THREE.Vector3(0, 25, 90));
        
        var redMaterial = new THREE.MeshLambertMaterial({color: 0xff0000});
        var greenMaterial = new THREE.MeshLambertMaterial({color: 0x44ff44});
        playerBall.addEventListener('collision', function (other) {
            var otherUserData = other.userData || {};
            if (otherUserData.badguy) {
                other.material = redMaterial;
                deltaScore(-1000);
            }
            if (otherUserData.goodguy) {
                other.material = greenMaterial;
                deltaScore(9000);
            }
        });
    });
}

function makePlayerLayer(){
    var layerMaterial = new THREE.MeshBasicMaterial({color: 0x555555, transparent: true, opacity: .54});
    playerLayer = new THREE.Mesh(new THREE.BoxGeometry(wallWidth, 1, wallWidth), layerMaterial);
    playerLayer.receiveShadow = true;
    playerLayer.castShadow = true;
    playerLayer.position.set(0,0,0);
    scene.add(playerLayer);
}

function makeGameBox(){
    // --------------------------------------------------------------------------
	// Make the box the game is going to take place in.
    gameBoxMaterial = Physijs.createMaterial(
        
        new THREE.MeshLambertMaterial({ map: myLoadTexture( 'images/textures/wood.jpg' )}),
        .8, // high friction
        1 // low restitution
    );
    gameBoxMaterial.map.wrapS = gameBoxMaterial.map.wrapT = THREE.RepeatWrapping;
    gameBoxMaterial.map.repeat.set( 3, 3);
    
    // ------------------------------------------- ^
    jQuery.map([
        {r: 0 + Math.PI, translate: {x: 0, z: 1}},
        {r: Math.PI/2 + Math.PI, translate: {x: 1, z: 0}},
        {r: Math.PI + Math.PI, translate: {x: 0, z: -1}},
        {r: 3*Math.PI/2 + Math.PI, translate: {x: -1, z: 0}},
    ], function (wallInfo) {
        
        
        var wall = new Physijs.PlaneMesh(
		  new THREE.PlaneGeometry( 
                wallWidth,
                wallHeight),
            gameBoxMaterial,
            0);

        wall.rotation.set(0, wallInfo.r, 0);
        wall.position.set(wallInfo.translate.x * wallWidth/2, wallHeight/2 - wallHeight/24, wallInfo.translate.z * wallWidth/2);
        //console.log('Make wall with rotation=' + wallInfo.r + ' and position=' + wall.position.toArray().join(','));

		wall.castShadow = true;
		wall.receiveShadow = true;
		scene.add(wall);
    });
}

var pointLightYOffset = -70000;
function makePointLight(){
    jQuery.map([
        /*
        {xOffset: 0, yOffset: pointLightYOffset, zOffset: 300, color: 0xff0000, distanceOffset: 0},
        {xOffset: 200 * Math.cos(Math.PI * -1/6), yOffset: pointLightYOffset, zOffset: 300 * Math.sin(Math.PI * -1/6), color: 0x00ff00, distanceOffset: 0},
        {xOffset: 200 * Math.cos(Math.PI * -5/6), yOffset: pointLightYOffset, zOffset: 300 * Math.sin(Math.PI * -1/6), color: 0x0000ff, distanceOffset: 0},*/
        //{yOffset: -wallHeight / 10, zOffset: 0, color: 0xffffff, distance: wallHeight/ 8, decay: 2, posChange: false},
        //{yOffset: wallHeight/ 1.5, color: 0x99ffff, distance: wallHeight / 2, decay: 1, intensity: 0.5},
        //{yOffset: wallHeight / 4, color: 0xffff77, distance: wallHeight, decay: 0},
        {yOffset: 200, color: 0xffffff, distance: wallHeight / 1.5, decay: 0, intensity: .25},
        {yOffset: wallHeight / 16, color: 0xffffff, distance: wallHeight / 4 , decay: 0, intensity: .3},
        {yOffset: -wallHeight / 5, color: 0xffffff, distance: wallHeight , decay: 0, intensity: .5}
  ], function (lightInfo) {
        var light = new THREE.PointLight(
            lightInfo.color, 
            lightInfo.intensity || 1, // Intensity
            lightInfo.distance, // Distance
            lightInfo.decay); // decay
         light.castShadow = true;
        light.shadowCameraNear = 1;
        light.shadowCameraFar = wallHeight * 4;
        light.shadowDarkness = 0.5;
        light.shadowMapWidth = 2048;
        light.shadowMapHeight = 2048;

      var setLightPosition = function () {
          light.position.set(0, 
                             lightInfo.yOffset, 
                             0);
      };
      setLightPosition();
      scene.addEventListener('update', setLightPosition);
      scene.add(light);
  });
     }

/*
funciton makeTextMesh(){
    var i = 1;
}

function makeText3D(){
    var textMaterial = new THREE.MeshPhongMaterial({
        color: 0xdddddd
    });
    var textGeom = new THREE.TextGeometry( 'Hello World!', {
        font: 'harabara' // Must be lowercase!
        , size: 10
        , height: 10
    });
    var textMesh = new THREE.Mesh( textGeom, textMaterial );

    scene.add( textMesh );
}
*/

function makeSkyBox(){
    var cubeMap = new THREE.CubeTexture( [] );
    cubeMap.format = THREE.RGBFormat;

    myLoadImage( 'images/textures/sky/skyboxsun25degtest.png', function ( image ) {
        if (!image)
            throw new Error('Unable to load something.');
        console.log(image);

        var getSide = function ( x, y ) {

            var size = 1024;

            var canvas = document.createElement( 'canvas' );
            canvas.width = size;
            canvas.height = size;

            var context = canvas.getContext( '2d' );
            context.drawImage( image, - x * size, - y * size );

            return canvas;

        };
        cubeMap.images[ 0 ] = getSide( 2, 1 ); // px
        cubeMap.images[ 1 ] = getSide( 0, 1 ); // nx
        cubeMap.images[ 2 ] = getSide( 1, 0 ); // py
        cubeMap.images[ 3 ] = getSide( 1, 2 ); // ny
        cubeMap.images[ 4 ] = getSide( 1, 1 ); // pz
        cubeMap.images[ 5 ] = getSide( 3, 1 ); // nz
        cubeMap.needsUpdate = true;
    } );

    var cubeShader = THREE.ShaderLib[ 'cube' ];
    cubeShader.uniforms[ 'tCube' ].value = cubeMap;

    var skyBoxMaterial = new THREE.ShaderMaterial( {
        fragmentShader: cubeShader.fragmentShader,
        vertexShader: cubeShader.vertexShader,
        uniforms: cubeShader.uniforms,
        depthWrite: false,
        side: THREE.BackSide
    } );

    var skyBox = new THREE.Mesh(
        new THREE.BoxGeometry( wallWidth * 4, wallHeight * 1.5, wallWidth * 4 ),
        skyBoxMaterial
    );

    scene.add( skyBox );
}

var light;

postprocessing = { enabled : true };
bgColor = 0x000511;
sunColor = 0xffee00;

function createScene()
{
    
    setSceneBasics();
    
    var light = new THREE.AmbientLight( 0x404040 ); // soft white light
    scene.add( light );
    
    setGravity();
   
    return makePlayerBox().then(function () {
    
    //makePlayerLayer();
    
    //makeText3D();
    
    //scene.add( new THREE.AmbientLight( 0x404040 ) );
    
    
    makeSkyBox();
    
    makeGameBox(); 
    
	makePointLight();
	
	
	// MAGIC SHADOW CREATOR DELUXE EDITION with Lights PackTM DLC
	renderer.shadowMap.enabled = true;
    //renderer.shadowMapType = THREE.PCFSoftShadowMap; // snooth shadows

    /* Call the playerBallMovement() once every physics (simulation) frame. */

    scene.addEventListener('update', animate);
    });
}

function draw()
{	
	// Physijs simulate
    if(!Key.isDown(Key.F)) {
	   scene.simulate(); // run physics
    }
	// draw THREE.JS scene
    
    cameraPosition();

    var i;
    for (i = 0; i < avalanche.length; i++)
        avalanche[i].onrender();

	renderer.render(scene, camera);
    
    // loop draw function call
	requestAnimationFrame(draw);
}

function makeMirrorSphere(){
    // MIRROR
    /*
    var cubeGeom = new THREE.BoxGeometry(10, 10, 10);
	mirrorCubeCamera = new THREE.CubeCamera( .1, 5000, 100 );
    scene.add(mirrorCubeCamera);
	// mirrorCubeCamera.renderTarget.minFilter = THREE.LinearMipMapLinearFilter;
	var mirrorCubeMaterial = new THREE.MeshBasicMaterial( { envMap: mirrorCubeCamera.renderTarget } );
	mirrorCube = new THREE.Mesh( cubeGeom, mirrorCubeMaterial );
    mirrorCube.position.set(0,-10000,0);
    mirrorCube.rotation.set(-Math.PI/3, 0, Math.PI/3);
    mirrorCubeCamera.position = mirrorCube.position;
    mirrorCubeCamera.rotation = mirrorCube.rotation;
	scene.add(mirrorCube);*/
    
    // Sphere
    var sphereGeom =  new THREE.SphereGeometry( Math.random() * wallWidth/40 + wallWidth/20, 20, 20 ); // radius, segmentsWidth, segmentsHeight
	var mirrorSphereCamera = new THREE.CubeCamera( 0.1, 5000, 100 );
	// mirrorCubeCamera.renderTarget.minFilter = THREE.LinearMipMapLinearFilter;
	//scene.add( mirrorSphereCamera );
	var mirrorSphereMaterial = new THREE.MeshBasicMaterial( { envMap: mirrorSphereCamera.renderTarget } );
	var mirrorSphere = new Physijs.SphereMesh( sphereGeom, mirrorSphereMaterial );
	mirrorSphere.position.set(Math.random() * (wallWidth/2), wallHeight - 200, Math.random() * (wallWidth/2));
    mirrorSphere.add(mirrorSphereCamera);
	//mirrorSphereCamera.position = mirrorSphere.position;
    mirrorSphere.userData = { goodguy: true };
	scene.add(mirrorSphere);

    avalanche.push({
        object: mirrorSphere,
        onrender: function () {
            mirrorSphere.visible = false;
            mirrorSphereCamera.updateCubeMap(renderer, scene);
            mirrorSphere.visible = true;
        },
        remove: function () {
            scene.remove(mirrorSphere);
            mirrorSphere.remove(mirrorSphereCamera);
        }
    });
}

var imagesI = 0;
function createAvalanche(){
    
    var chanceToMake = Math.random() * chancePerFrame;
    if (chanceToMake > 2)
        return;
    if (chanceToMake > 1 && Math.random() * 100 < 5) {
        makeMirrorSphere();
        return;
    }
    // Materials
        images.then(function (images) {
    var avalancheMaterial = Physijs.createMaterial(
        new THREE.MeshLambertMaterial({ map: images[imagesI++] }),
        10, // high friction
        .2 // low restitution
    );
		imagesI %= images.length;
	
    var obj;
    if (1 < Math.random() * 2) {
        obj = new Physijs.SphereMesh( 

           new THREE.SphereGeometry( 
              Math.random() * wallWidth/8 + wallWidth/80, 
              100,
              100), 
            avalancheMaterial,
            1); // Gravity

        obj.receiveShadow = true;
        obj.castShadow = true;
        obj.position.set(Math.random() * (wallWidth/2), wallHeight - 200, Math.random() * (wallWidth/2));
    }else{
        obj = new Physijs.BoxMesh( 

           new THREE.BoxGeometry( 
              Math.random() * wallWidth/8 + wallWidth/10, 
              Math.random() * wallWidth/8 + wallWidth/10,
              Math.random() * wallWidth/8 + wallWidth/10), 
            avalancheMaterial,
            1); // Gravity

        obj.receiveShadow = true;
        obj.castShadow = true;
        obj.position.set(Math.random() * (wallWidth/2), wallHeight - 200, Math.random() * (wallWidth/2));
    }
            obj.userData = { badguy: true };
	scene.add(obj);
	
            // // add the sphere to the scene
    avalanche.push({
        object: obj,
        onrender: function () {},
        remove: function () {
            scene.remove(obj);
        }
    });
    // Slow down the ball
	obj.setDamping( 0, .9);
    
    obj.setAngularFactor(new THREE.Vector3(2,2,2));
    
    obj.rotation.set(new THREE.Vector3(0, 25, 90));
        });
}

function deleteAvalanche(){
    for (i = avalanche.length - 1; i >= 0; i--){
        var entry = avalanche[i];
        if (entry.object.position.y < - wallWidth * 7) {
            avalanche.splice(i, 1);
            entry.remove();
        }
    }
}

function keepPlayerInBox(){
    // If player is too far out then put it back in
    var checkInt = playerBall.position.x + playerBall.width;
    if (checkInt > wallWidth/2){
        playerBall.__dirtyPosition = true;
        playerBall.position.x = checkInt;
    }
    if (-checkInt < wallWidth/2){
        playerBall.__dirtyPosition = true;
        playerBall.position.x = -checkInt;
    }
    checkInt = playerBall.position.z + playerBall.width;
    if (checkInt > wallWidth/2){
        playerBall.__dirtyPosition = true;
        playerBall.position.z = checkInt;
    }
    if (-checkInt < wallWidth/2){
        playerBall.__dirtyPosition = true;
        playerBall.position.z = -checkInt;
    }
}

var nextScore = 1;
var animationsPerScore = 30;

// Handles player's movement
function animate()
{
    if (!--nextScore) {
        deltaScore(1);
        nextScore = animationsPerScore;
    }
    createAvalanche();
    deleteAvalanche();
    // Keep player ball in the same place
    // --------------------- Player movement ------
    if (playerBall.position.y < 0){
        playerBall.applyCentralForce(new THREE.Vector3(0,1000,0));
    }
    
    var playerMovementAccel = wallWidth * 50;
	// move left
	if (Key.isDown(Key.A))		
	{
		// Move left
		playerBall.applyCentralForce(new THREE.Vector3(0, 0, playerMovementAccel));
	}	
	// move right
	if (Key.isDown(Key.D))
	{
		playerBall.applyCentralForce(new THREE.Vector3(0, 0, -playerMovementAccel));
	}
    if (Key.isDown(Key.W))
    {
        playerBall.applyCentralForce(new THREE.Vector3(playerMovementAccel, 0, 0));
    }
    if (Key.isDown(Key.S))
    {
        playerBall.applyCentralForce(new THREE.Vector3(-playerMovementAccel, 0, 0));
    }
    if(Key.isDown(Key.SPACE)) {
        playerBall.applyCentralForce(new THREE.Vector3(0,2000,0));
    }
    
    
    
    // Rotational forces 
    var boxSideLengthHalf = 50;
    var strength = 25;
    // Position and rotation
    if (Key.isDown(Key.Z)) {
        playerBall.applyForce(new THREE.Vector3(0, strength, 0).applyEuler(playerBall.rotation), new THREE.Vector3(-boxSideLengthHalf, 50, -boxSideLengthHalf).applyEuler(playerBall.rotation).add(playerBall.position));
    }
    if (Key.isDown(Key.X)) {
        playerBall.applyForce(new THREE.Vector3(0, strength, 0).applyEuler(playerBall.rotation), new THREE.Vector3(boxSideLengthHalf, 50, -boxSideLengthHalf).applyEuler(playerBall.rotation).add(playerBall.position));
    }
    if (Key.isDown(Key.C)) {
        playerBall.applyForce(new THREE.Vector3(0, strength, 0).applyEuler(playerBall.rotation), new THREE.Vector3(-boxSideLengthHalf, 50, boxSideLengthHalf).applyEuler(playerBall.rotation).add(playerBall.position));
    }
    if (Key.isDown(Key.V)) {
        playerBall.applyForce(new THREE.Vector3(0, strength, 0).applyEuler(playerBall.rotation), new    THREE.Vector3(boxSideLengthHalf, 50, boxSideLengthHalf).applyEuler(playerBall.rotation).add(playerBall.position));
    }
    /*
	paddle1.scale.y += (1 - paddle1.scale.y) * 0.2;	
	paddle1.scale.z += (1 - paddle1.scale.z) * 0.2;	
	paddle1.position.y += paddle1DirY;
	*/
    //keepPlayerInBox();
}

// Handles camera and lighting logic
function cameraPosition()
{
    var camPosMax = wallWidth/2 - 1000;
    // Make the camera cooler and stuff
    var posXMax = /*(playerBall.position.x > 0)? Math.min(camPosMax, playerBall.position.x): Math.max(-camPosMax, playerBall.position.x)*/ playerBall.position.x;
    var posZMax = /*(playerBall.position.z >0)? Math.min(camPosMax, playerBall.posision.z): Math.max(-camPosMax, playerBall.position.z)*/ playerBall.position.z;
    var adfs = playerBall.getLinearVelocity();
    
    camera.position.set(
        posXMax - posXMax,
        playerBall.position.y - wallHeight/4, 
        posZMax - posZMax);
    camera.up.set(1, 0, 0);
    camera.lookAt(new THREE.Vector3(0.25*posXMax, playerBall.position.y, 0.25*posZMax));
    /*
	// we can easily notice shadows if we dynamically move lights during the game
	//spotLight.position.x = playerBall.position.x;
   // spotLight.position.y = playerBall.position.y + 200;
	//spotLight.position.z = playerBall.position.z;
	// Camera movement IJKL
    if (Key.isDown(Key.I)) {
        camera.rotation.x += 1 * Math.PI/180;
    }
    if (Key.isDown(Key.K)) {
        camera.rotation.x -= 1 * Math.PI/180;
    }
    if (Key.isDown(Key.J)) {
        camera.rotation.y  += 1 * Math.PI/180;
    }
    if (Key.isDown(Key.L)) {
        camera.rotation.y -= 1 * Math.PI/180;
    }
	// move to behind the player's paddle
	camera.position.x = playerBall.position.x - 300;
	camera.position.y = playerBall.position.y + 400;
	camera.position.z = playerBall.position.z + 500;
   */
    //camera.position.x = playerBall.position.x - 300;
}




