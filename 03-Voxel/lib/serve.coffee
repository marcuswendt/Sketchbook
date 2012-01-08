http		= require 'http'
url			= require 'url'
path		= require 'path'
fs			= require 'fs'
mime 		= require 'mime'
{log} 		= require 'util'

basepath = '.'
port = 8000

http.createServer( (request, response) ->

	uri = url.parse(request.url).pathname
	filePath = path.join(basepath, uri)

	
	path.exists filePath, (exists) ->
		if exists
			if fs.statSync(filePath).isDirectory()
				filePath += 'index.html'

			log 'serving '+ filePath
			
			fs.readFile filePath, 'binary', (error, content) ->
				if error
					response.writeHead(500, {"Content-Type": "text/plain"})
					response.write error +"\n"
					response.end
				else
					type = mime.lookup(filePath)
					response.writeHead(200, { 'Content-Type': type })
					response.end(content, 'utf-8')
					# response.write(content, 'binary')
					# response.end
		else
			response.writeHead 404, {"Content-Type": "text/plain" }
			response.write "404 Not Found\n"
			response.end

).listen port

log "Server running at http://127.0.0.1:#{port}/"
