#pragma once

class RunningStats {
public:
    RunningStats() :
        m_n(0),
        m_oldM(0.0),
        m_newM(0.0),
        m_oldS(0.0),
        m_newS(0.0)
    {}

    void Clear()
    {
        m_n = 0;
    }

    void Push(long double x)
    {
        m_n++;

        // See Knuth TAOCP vol 2, 3rd edition, page 232
        if (m_n == 1)
        {
            m_oldM = m_newM = x;
            m_oldS = 0.0;
        }
        else
        {
            m_newM = m_oldM + (x - m_oldM) / m_n;
            m_newS = m_oldS + (x - m_oldM) * (x - m_newM);

            // set up for next iteration
            m_oldM = m_newM;
            m_oldS = m_newS;
        }
    }

    unsigned long long NumDataValues() const
    {
        return m_n;
    }

    long double Mean() const
    {
        return (m_n > 0.0) ? m_newM : 0.0;
    }

    long double Variance() const
    {
        return ((m_n > 1) ? m_newS / (m_n - 1) : 0.0);
    }

    long double SquaredStandardScore(long double rawScore) const
    {
        return (m_n > 1) ? (rawScore - Mean()) * (rawScore - Mean()) / Variance() : 0.0;
    }

    bool IsSigmaEvent(long double rawScore, long double sigma) const
    {
        return (m_n > 1) ?
            sigma * sigma * Variance() < (rawScore - Mean()) * (rawScore - Mean())
            : false;
    }

private:
    unsigned long long m_n;
    long double m_oldM, m_newM, m_oldS, m_newS;
};