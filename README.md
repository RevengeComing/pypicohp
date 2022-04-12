pypicohp is a Python binding for the h2o HTTP parser picohttpparser.

The package is available on PyPI: `pip install pypicohp`.

# APIs

pypicohp currently supports for parsing http requests and will support responses in future merge requests.

```python
from pypico.request import Request

REQUEST = b"""POST /test HTTP/1.1\r
User-Agent:Test\r
Host:www.test.com\r
Accept-Language:en-us\r
Content-Length:4\r\n\r
Test"""

r = Request()
pret = r.feed_data(REQUEST)

r,get_method() # returns b"POST"
r.get_path() # returns b"/test"
r.get_headers() # returns python dict {"User-Agent": "Test", "Host": "www.test.com", "Accept-Language": "en-us", "Content-Length": "4"}
r.get_http_version() # returns "1.1"
body = REQUEST[pret:]
```

# Development

In near future parsing http response will be added. Also, support for chunked transfer encoding will be added.

For contribution:

1. Clone this repository.
2. Create a python environment.
3. Run `sudo make build` for local development or run `make install` to install and test on python environment.
4. Run `make test` to test.
