


def __main__(periode_but_ms):
    for prescaler in [1, 4, 16, 64]:
        for postscaler in range(1, 16):
            for pr2 in range(1, 250):
                toverflow_ms = (pr2+1)*((prescaler * postscaler)/(8000000/4))*1000
                if (toverflow_ms == periode_but_ms) and (1000%periode_but_ms == 0):
                    print(f"pour une période de {periode_but_ms}ms\n prescaler : {prescaler}, postscaler : {postscaler}, pr2 : {pr2}")
                    print(f" {1000/periode_but_ms} fois ce nombre de ms dans 1000")
                    return

for i in range (1,500):
    __main__(i)
