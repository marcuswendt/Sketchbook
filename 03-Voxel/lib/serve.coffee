http		= require "http"
path		= require 'path'
fs			= require 'fs'
{log} 		= require 'util'


http.createServer( (request, response) ->
	filePath = '.' + request.url
	if filePath == './'
	   filePath = './index.html'

	log 'serving '+ filePath
	
	path.exists filePath, (exists) ->
		if exists
			fs.readFile filePath, (error, content) ->
				if error
					response.writeHead(500)
					response.end 'Server Error'
				else
					response.writeHead(200, { 'Content-Type': 'text/html' })
					response.end(content, 'utf-8')
		else
			response.writeHead(404)
			response.end
).listen 8000

log "Server running at http://127.0.0.1:8000/"
