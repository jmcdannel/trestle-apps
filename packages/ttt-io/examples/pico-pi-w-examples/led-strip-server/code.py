import board
import neopixel
import socketpool
import wifi
import adafruit_led_animation.animation.sparkle as sparkle_animation

from adafruit_httpserver import (
    Server,
    Route,
    as_route,
    Request,
    Response,
    GET,
    POST,
    PUT,
    DELETE,
    JSONResponse,
)

pool = socketpool.SocketPool(wifi.radio)
server = Server(pool, "/static", debug=True)
pixel = neopixel.NeoPixel(board.GP0, 30)

@server.route("/led", POST)
def change_neopixel_color_handler_post_json(request: Request):
    """Changes the color of the built-in NeoPixel using JSON POST body."""

    data = request.json()  # e.g {"r": 255, "g": 0, "b": 0}
    print(data)
    start, end, command, r, g, b = (
        data.get("start"),
        data.get("end"),
        data.get("command"),
        data.get("r"),
        data.get("g"),
        data.get("b"),
    )

    if command == "color":
        for i in range(start, end):
            pixel[i] = (r, g, b)
        return Response(request, f"Changed NeoPixel to color ({r}, {g}, {b})")
    else:
        return Response(request, f"Unknown command ({command}")


print("Starting HTTP Server at:")
print(str(wifi.radio.ipv4_address))
server.serve_forever(str(wifi.radio.ipv4_address))
