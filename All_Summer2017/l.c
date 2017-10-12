#ifdef kernel_default

    void execute_kernel()
    {
        ulong_t r, c;
        ulong_t NODES;
        node* nodes;
        ulong_t checksum;

        /* PIM Scalar Registers
        SREG[0]: nodes
        SREG[1]: ---
        SREG[2]: number of nodes
        SREG[3]: retval
        */
        nodes = (node*)PIM_SREG[0];
        NODES = PIM_SREG[2];
        pim_print_hex("NODES", NODES);
        checksum = 0;

        for ( r=0; r<NODES; r++ )
            for ( c=0; c<nodes[r].out_degree; c++ ) // for node.successors
            {
                node*u = &nodes[r];
                node*v = nodes[r].successors[c];
                ulong_t w = nodes[r].weights[c];
                if ( u->distance != NC && v->distance > u->distance + w )
                {
                    v->distance = u->distance + w;
                    checksum += w;
                }
            }

        pim_print_hex("checksum", checksum);
        PIM_SREG[3] = checksum;
    }

#endif
