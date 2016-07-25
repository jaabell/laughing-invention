enum tensor_index { i, j, k, l, m, n, p, q };

/*
 * class tensor2 - a second order tensor with indices I and J.
 */
template<tensor_index I, tensor_index J>
class tensor2
{
    public:
        // null constructor
        explicit tensor2()
        {
        }
        // constant constructor
        explicit tensor2(double d)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    data[i][j] = d;
                }
        }
        // Copy a tensor with different indices (change indices)
        template<tensor_index P, tensor_index Q>
        explicit tensor2(tensor2<P, Q>& d)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    data[i][j] = d(i + 1, j + 1);
                }
        }
        // accessor
        double& operator() (int i, int j)
        {
            return data[i - 1][j - 1];
        }
    private:
        // Hard code 3 here... This could also be a template parameter.
        double data[3][3];
};

/*
 * class tensor4 - a forth order tensor with indices I, J, K and L.
 */
template<tensor_index I, tensor_index J, tensor_index K, tensor_index L>
class tensor4
{
    public:
        // null constructor
        explicit tensor4()
        {
        }
        // const constructor
        explicit tensor4(double d)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    for (int k = 0; k < 3; k++)
                        for (int l = 0; l < 3; l++)
                        {
                            data[i][j][k][l] = d;
                        }
        }
        // accessor
        double& operator() (int i, int j, int k, int l)
        {
            return data[i - 1][j - 1][k - 1][l - 1];
        }
    private:
        // Again, 3 is hard coded.
        double data[3][3][3][3];
};

/*
 * Case 1 : 4th order multiplied by 2nd order r_IJ = a_IJKL*b_KL
 */
template<tensor_index I, tensor_index J, tensor_index K, tensor_index L>
inline tensor2<I, J>
operator* (tensor4<I, J, K, L>& a, tensor2<K, L>& b)
{
    tensor2<I, J> r(0.0);

    for (int i = 1; i <= 3; i++)
    {
        for (int j = 1; j <= 3; j++)
        {
            for (int k = 1; k <= 3; k++)
                for (int l = 1; l <= 3; l++)
                {
                    r(i, j) += a(i, j, k, l) * b(k, l);
                }
        }
    }

    return r;
}
/*
 * Case 2 : 4th order multiplied by 2nd order r_IK = a_IJKL*b_JL
 */
template<tensor_index I, tensor_index J, tensor_index K, tensor_index L>
inline tensor2<I, K>
operator* (tensor4<I, J, K, L>& a, tensor2<J, L>& b)
{
    tensor2<I, K> r(0.0);

    for (int i = 1; i <= 3; i++)
    {
        for (int k = 1; k <= 3; k++)
        {
            for (int j = 1; j <= 3; j++)
                for (int l = 1; l <= 3; l++)
                {
                    r(i, k) += a(i, j, k, l) * b(j, l);
                }
        }
    }

    return r;
}

int main()
{
    // some variables to play with...
    tensor2<k, l> e;
    tensor2<j, l> b;
    tensor2<i, j> s;
    tensor2<i, k> K;
    tensor4<i, j, k, l> E;

    s = E * e;  // Case 1.
    tensor2<p, q> w(s); // Copy with change of indices
    //w = E*e;  // Compile error... w_pq = E_ijkl*e_kl???
    K = E * b;  // Case 2.
}

