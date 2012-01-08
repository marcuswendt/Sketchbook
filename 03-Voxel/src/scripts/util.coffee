# define ['cs!util'], (util) ->
#   render: (body) ->
#     body.appendChild util.toDom('<b>This is a rendered view</b>')

# define ['cs!util'], () ->
#   render: (body) ->
#     body.appendChild util.toDom('<b>This is a rendered view</b>')

define
	test: (str) ->
		console.log('test: '+ str)