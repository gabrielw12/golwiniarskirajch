class Rules {
public:
    virtual ~Rules() = default;
    virtual bool next_state(bool is_alive, int neighbours) const = 0;

};

/**
 * Zasady dla klasycznej wersji Conway'a: jeżeli żywa komórka jest otoczona 2 lub 3 sąsiadami to przeżywa do
 * następnej iteracji, w innym przypadku umiera.
 * Jeżeli martwa komórka ma 3 sąsiadów, w następnej iteracji ożywa.
 */