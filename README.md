# C Webserver

A simple HTTP server that serves html files.
This project is mainly focused on the routing system of a webserver,
the routing is file-system based, all pages are stored in the `pages` directory.

Example:
```
pages:
	- [FILE] index.js 	# the home route (domain.com/)
	- [FILE] about.js 	# the about route (domain.com/about)
	- [DIR] services 	  # nested route
		- index.js 		    # the home of the services route (domain.com/services/)
		- service1.js 	  # a nested route (domain.com/service/service1)
```

### TODO List
- [x] basic routing system
- [x] add support for a custom 404 page
- [x] nested routes
