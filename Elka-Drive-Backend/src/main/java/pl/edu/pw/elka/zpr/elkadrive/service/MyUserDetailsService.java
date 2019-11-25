package pl.edu.pw.elka.zpr.elkadrive.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import pl.edu.pw.elka.zpr.elkadrive.domain.UserIdent;
import pl.edu.pw.elka.zpr.elkadrive.repository.UserRepository;

import java.util.Optional;

@Service
public class MyUserDetailsService implements UserDetailsService {
    private UserRepository userRepository;

    @Autowired
    public MyUserDetailsService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        Optional<UserIdent> user = userRepository.findByUsername(username);
        return user
                .orElseThrow(() -> new UsernameNotFoundException("User '" + username + "' not found"));
    }
}
