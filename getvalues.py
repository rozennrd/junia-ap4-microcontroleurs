
periode_but_ms = 1

def __main__():
    for prescaler in [1, 4, 16, 64]:
        for postscaler in range(1, 16):
            for pr2 in range(1, 250):
                toverflow_ms = (pr2+1)*((prescaler * postscaler)/(8000000/4))*1000
                if toverflow_ms == periode_but_ms:
                    print(f"pour une période de {periode_but_ms}ms\n prescaler : {prescaler}, postscaler : {postscaler}, pr2 : {pr2}")
                    return

__main__()
