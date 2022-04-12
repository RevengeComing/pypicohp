from pypicohp.request import Request

from unittest import TestCase


REQUEST = b"""POST /test HTTP/1.1\r
User-Agent:Test\r
Host:www.test.com\r
Accept-Language:en-us\r
Content-Length:4\r\n\r
Test"""


class TestRequestParser(TestCase):

    def test_parser_request(self):
        p = Request()

        finished = p.feed_data(REQUEST)
        self.assertEqual(finished, True)
        self.assertEqual(p.get_method(), b'POST')
        self.assertEqual(p.get_http_version(), b'1.1')
        self.assertEqual(p.get_body(), b'Test')
        self.assertEqual(p.get_path(), b'/test')
        self.assertEqual(p.get_headers(), {
            b'User-Agent': b'Test',
            b'Host': b'www.test.com',
            b'Accept-Language': b'en-us',
            b'Content-Length': b'4',
        })

    def test_parser_request_partial(self):
        p = Request()

        finished = p.feed_data(REQUEST[:20])
        self.assertEqual(finished, False)
        finished = p.feed_data(REQUEST[20:])
        self.assertEqual(finished, True)
        self.assertEqual(p.get_method(), b'POST')
        self.assertEqual(p.get_http_version(), b'1.1')
        self.assertEqual(p.get_body(), b'Test')
        self.assertEqual(p.get_path(), b'/test')
        self.assertEqual(p.get_headers(), {
            b'User-Agent': b'Test',
            b'Host': b'www.test.com',
            b'Accept-Language': b'en-us',
            b'Content-Length': b'4',
        })
