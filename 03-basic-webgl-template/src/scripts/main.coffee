define [
  'cs!util',
  'lib/dat.gui', 'lib/dat.color',
  'lib/Three', 'lib/ThreeWebGL',
  'lib/RequestAnimationFrame',
  'lib/Detector'
], (util, gui) ->
	log = util.log
	err = util.err

	if(!Detector.webgl) 
		Detector.addGetWebGLMessage
		err 'VOXEL: Couldnt initialize WebGL'
		return

	log 'main'
	[camera, scene, renderer, geometry, material, mesh] = [null]

	init = (containerId) ->
		log 'creating scene'

		scene = new THREE.Scene

		camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 10000)
		camera.position.z = 1000
		scene.add(camera)

		geometry = new THREE.CubeGeometry 200, 200, 200;
		material = new THREE.MeshBasicMaterial( { color: 0xff0000, wireframe: true } );

		mesh = new THREE.Mesh(geometry, material)
		scene.add(mesh)

		# renderer = new THREE.CanvasRenderer
		renderer = new THREE.WebGLRenderer( {antialias: false} )
		renderer.setSize(window.innerWidth, window.innerHeight)

		#document.body.appendChild(renderer.domElement)
		document.getElementById(containerId).appendChild(renderer.domElement)

	render = ->
		# log 'render'
		mesh.rotation.x += 0.01
		mesh.rotation.y += 0.02

		renderer.render(scene, camera)

	animate = ->
		requestAnimationFrame(animate)
		render()

	init('canvas')
	animate()
