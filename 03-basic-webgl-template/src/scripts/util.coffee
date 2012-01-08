# define ['cs!util'], (util) ->
#   render: (body) ->
#     body.appendChild util.toDom('<b>This is a rendered view</b>')

# define ['cs!util'], () ->
#   render: (body) ->
#     body.appendChild util.toDom('<b>This is a rendered view</b>')

define 
	log: (str) -> console.log(str)
	err: (str) -> console.error(str)