#ifndef _DIMENSION_H_
#define _DIMENSION_H_

class Dimension {
    public:
        Dimension(int i, int j);
        Dimension(const Dimension& origen);
        virtual ~Dimension();
        bool esValida();
        int x() { return filas; }
        int y() { return cols; }
        Dimension& operator=(Dimension& otra);
        friend int operator==(const Dimension& una, const Dimension& otra);
        friend int operator!=(const Dimension& una, const Dimension& otra);
        friend bool sonAdyacentesFila(Dimension& una, Dimension& otra);
        friend bool sonAdyacentesCol(Dimension& una, Dimension& otra);
        friend bool sonAdyacentes(Dimension& una, Dimension& otra);
    private:
        int filas;
        int cols;
};
#endif
